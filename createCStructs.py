#!/usr/bin/python
import sys, xmltodict, json
from collections import OrderedDict

class ParseXML:

    def __init__(self):
        self.xml_schema = "main_schema.xml" #main_schema.xml default_schema.xml
        self.file_h = open("xml_data.h", "w")
        self.file_c = open("xml_data.c", "w")
        self.externs_for_h_footer = ""


    def get_xml(self):
        print "reading in xml from file %s" % (self.xml_schema)
        with open (self.xml_schema, "r") as xmlfile:
            xml=xmlfile.read()
        xmlfile.closed
        return xml


    def parse_xml(self, xml):
        #print "parsing xml file contents"
        parsed_xml = json.loads(json.dumps(xmltodict.parse(xml)))
        #print "parsed xml: %s" % (parsedxml)
        return parsed_xml


    def get_sub_structures(self, parsed_xml):
        partitions = parsed_xml.get('ARINC_653_Module').get('Partition', None)
        partition_memory = parsed_xml.get('ARINC_653_Module').get('Partition_Memory', None)
        self.major_frame = parsed_xml.get('ARINC_653_Module').get('Module_Schedule', None)
        if self.major_frame:
            partition_schedule = parsed_xml.get('ARINC_653_Module').get('Module_Schedule').get('Partition_Schedule', None)
        channels = parsed_xml.get('ARINC_653_Module').get('Connection_Table').get('Channel', None)
        sub_structures = [partitions, partition_memory, partition_schedule, channels]
        return sub_structures


    def write_to_file_h(self, data):        
        self.file_h.write(data)


    def write_to_file_c(self, data):        
        self.file_c.write(data)


    def write_file_h_header(self):
        template_h = open("xml_data_template.h", "r")
        for line in template_h.readlines():
            self.write_to_file_h(line)
        template_h.close()


    def write_file_h_footer(self):
        self.write_to_file_h("""#endif""")


    def write_file_c_header(self):
        self.write_to_file_c("""#include <stdint.h>

#include "xml_data.h"
#include "circular_buffer.h"
#include "arinc/queuing_port.h"

""")


    def create_sub_structure_structs(self, sub_structure):
        structs_string = ""
        sub_structure = self.put_dicts_in_list(sub_structure)
        no_of_sub_elements = len(sub_structure)
        for sub_element in sub_structure:
            sub_element_struct = self.construct_sub_element_struct(sub_element)            
            structs_string = structs_string + sub_element_struct
            
        partition = sub_element.get('@Criticality', None) 
        partition_memory = sub_element.get('Memory_Requirements', None)         
        partition_schedule = sub_element.get('Window_Schedule', None)    
        channels = sub_element.get('@ChannelName', None)    
        if partition:
            complete_struct = "partition_t partitions[%s] = {%s};\n\n" % (no_of_sub_elements + 1, structs_string) #+1 added 
            self.write_to_file_c(complete_struct)
            self.externs_for_h_footer = self.externs_for_h_footer + "\n\nextern partition_t partitions[%s]\n" % (no_of_sub_elements + 1)
        elif partition_memory:
            complete_struct = "const partition_memory partition_memorys[%s] = {%s};\n\n" % (no_of_sub_elements, structs_string)
            self.write_to_file_c(complete_struct)
            major_frame = self.major_frame.get('@MajorFrameSeconds', None)
            mf_wrapper = "const uint32_t majorFrameSeconds = %s\n\n;" % (major_frame)
            self.write_to_file_c(mf_wrapper)
            self.externs_for_h_footer = self.externs_for_h_footer + "extern partition_memory partition_memorys[%s]\n" % (no_of_sub_elements)
        elif partition_schedule:
            complete_struct = "const partition_schedule partition_schedules[%s] = {%s};\n\n" % (no_of_sub_elements, structs_string)
            self.write_to_file_c(complete_struct)
        elif channels:
            complete_struct = "channel_t connection_table[%s] = {%s};\n\n" % (no_of_sub_elements, structs_string)
            self.write_to_file_c(complete_struct)
            self.externs_for_h_footer = self.externs_for_h_footer + "extern channel_t connection_table[%s]\n" % (no_of_sub_elements + 1)


    def construct_sub_element_struct(self, sub_element):
        partitions = sub_element.get("@Criticality", ())
        memory_requirements = sub_element.get("Memory_Requirements", ())
        window_schedule = sub_element.get("Window_Schedule", ())
        channels = sub_element.get('Source', None)    
        if partitions:
            queuing_ports = self.put_dicts_in_list( sub_element.get("Queuing_Port", []) )
            sampling_ports = self.put_dicts_in_list( sub_element.get("Sampling_Port", []) )
            ports = queuing_ports + sampling_ports

            if not ports:
                no_of_ports = 0
                sub_element_struct, sub_element_name = self.partition_struct(sub_element, no_of_ports)
            else:                
                structs_string = ""
                no_of_ports = len(ports)
                sub_element_struct, sub_element_name = self.partition_struct(sub_element, no_of_ports)
                for port in ports:
                    if port.get("@MaxNbMessages", None):
                        get_list = ['@MaxNbMessages', '@MaxMessageSize', '@Direction', '@PortName']
                        get_list_tuple = self.return_get_tuple(port, get_list)
                        port_struct = """{{
    .is_queuing_port = true,
    .q_buf = [ MESSAGE_BUFFER({}, {}), ]
    .PORT_DIRECTION = {},
    .portname = \"{}\",
    }},""".format(*get_list_tuple)
                        port_struct = port_struct.replace ("[", "{")
                        port_struct = port_struct.replace ("]", "}")
                    else:
                        get_list = ['@PortName', '@MaxMessageSize', '@Direction', '@RefreshRateSeconds']
                        get_list_tuple = self.return_get_tuple(port, get_list)
                        port_struct = """{{
    .portname = \"{}\",
    .maxmessagesize = {},
    .direction = \"{}\",
    .refreshrateseconds = {},
    }},""".format(*get_list_tuple)

                    structs_string = structs_string + port_struct

                ports_wrapper = "port_t p_%s[%s] = {%s};\n\n" % (sub_element_name, no_of_ports, structs_string)
                self.write_to_file_c(ports_wrapper)


        elif memory_requirements:
            sub_element_struct, sub_element_name = self.partition_memory_struct(sub_element)
            structs_string = ""
            get_list = ['@Type', '@SizeBytes', '@Access', '@PhysicalAddress']
            memory_requirements = self.put_dicts_in_list(memory_requirements)
            no_of_mem_req = len(memory_requirements)
            for requirement in memory_requirements:
                get_list_tuple = self.return_get_tuple(requirement, get_list)
                mem_requirement_struct = """{{
    .type = \"{}\",
    .sizebytes = {},
    .access = \"{}\",
    .physicaladdress = \"{}\",
    }},""".format(*get_list_tuple)
                structs_string = structs_string + mem_requirement_struct
            mem_requirements_wrapper = "const memory_requirements memoryp_%s[%s] = {%s};\n\n" % (sub_element_name, no_of_mem_req, structs_string)
            self.write_to_file_c(mem_requirements_wrapper)


        elif window_schedule:
            sub_element_struct, sub_element_name = self.partition_schedule_struct(sub_element)
            structs_string = ""
            get_list = ['@WindowIdentifier', '@WindowStartSeconds', '@WindowDurationSeconds', '@PartitionPeriodStart']
            window_schedule = self.put_dicts_in_list(window_schedule)
            no_of_win_sch = len(window_schedule)
            for win in window_schedule:
                get_list_tuple = self.return_get_tuple(win, get_list)
                win_schedule_struct = """{{
    .windowidentifier = {},
    .windowstartseconds = {},
    .windowdurationseconds = {},
    .partitionperiodstart = {},
    }},""".format(*get_list_tuple)
                structs_string = structs_string + win_schedule_struct
            win_schedule_wrapper = "const window_schedule windowp_%s[%s] = {%s};\n\n" % (sub_element_name, no_of_win_sch, structs_string)
            self.write_to_file_c(win_schedule_wrapper)

        elif channels:
            ports = None
            source_list = []
            source = self.put_dicts_in_list( sub_element.get("Source", []) )
            if source:
                for src in source:
                    s = src.get("Standard_Partition", [])
                    source_list.append(s)
            source = source_list

            des_list = []
            destination = self.put_dicts_in_list( sub_element.get("Destination", []) )
            if destination:
                for des in destination:
                    d = des.get("Standard_Partition", [])
                    des_list.append(d)
            destination = des_list

            ports = source + destination
            no_of_ports = len(ports)
            sub_element_struct, sub_element_name = self.channel_struct(sub_element, no_of_ports)#change function

            structs_string = ""
            get_list = ['@PartitionName']
            no_of_channels = len(ports)
            for channel in ports:
                get_list_tuple = self.return_get_tuple(channel, get_list)

                #channel_struct = """{{&p_{}[0],}},""".format(*get_list_tuple)
                c = channel.get('@PartitionName', None) 
                channel_struct = """
    &p_%s[0],""" % (c)
                structs_string = structs_string + channel_struct
            channel_wrapper = "port_t *stio_channel_ports[%s] = {%s};\n\n" % (no_of_channels, structs_string)
            self.write_to_file_c(channel_wrapper)

        else:
            print "unknown sub_element: %s" % (sub_element)

        return sub_element_struct


    def partition_struct(self, sub_element, no_of_ports):
        #had to use @ sign as it appears in the data for some unknown reason
        part_id = sub_element.get('@PartitionIdentifier', None) 
        name = sub_element.get('@PartitionName', None).replace (" ", "_")
        crit_level = sub_element.get('@Criticality', None) 
        sys_part = sub_element.get('@SystemPartition', None) 
        entry = sub_element.get('@EntryPoint', None) 
        partition_struct = """{ 
    .IDENTIFIER = %s,
    .partitionname = \"%s\",
    .criticality = %s,
    .systempartion = %s,
    .entrypoint = %s,
    .nb_ports = %s,
    .ports = p_%s,
    },""" % (part_id, name, crit_level, sys_part, "&"+entry, no_of_ports, name)

        #this write the entry point to the .h file as it is a function needed to be defined
        self.write_to_file_h("""
void %s(void);""" % (entry))
        return partition_struct, name


    def partition_memory_struct(self, sub_element):
        part_id = sub_element.get('@PartitionIdentifier', None)
        name = sub_element.get('@PartitionName', None).replace (" ", "_")
        memory_arr = "memoryp_%s" % (name).replace (" ", "_")

        partition_memory_struct = """{
    .partitionidentifier = %s,
    .partitionname = \"%s\",
    .memory_arr = %s,
    },""" % (part_id, name, memory_arr)

        return partition_memory_struct, name

        
    def partition_schedule_struct(self, sub_element):
        part_id = sub_element.get('@PartitionIdentifier', None)
        name = sub_element.get('@PartitionName', "nope").replace (" ", "_")
        period_seconds = sub_element.get('@PeriodSeconds', None)
        period_duration_seconds = sub_element.get('@PeriodDurationSeconds', None)
        window_arr = "windowp_%s" % (name).replace (" ", "_")

        partition_schedule_struct = """{
    .partitionidentifier = %s,
    .partitionname = \"%s\",
    .peroidseconds = %s,
    .perioddurationseconds = %s,
    .window_arr = %s,
    },""" % (part_id, name, period_seconds, period_duration_seconds, window_arr)

        return partition_schedule_struct, name


    def channel_struct(self, sub_element, no_of_ports):
        channel_id = sub_element.get('@ChannelIdentifier', None)
        name = sub_element.get('@ChannelName', "nope").replace (" ", "_")
        channel_struct = """{
    .channelidentifier = %s,
    .channelname = \"%s\",
    .nb_ports = %s,
    .ports = %s,
    },""" % (channel_id, name, no_of_ports, name + "_ports" )

        return channel_struct, name


    def put_dicts_in_list(self, structure):
        if structure:
            if type(structure) == dict:
                structure_list = []
                structure_list.append(structure)
                structure = structure_list
        return structure


    def return_get_tuple(self, element, get_list):
        get_list_values = ()
        for get_item in get_list:            
            get_list_value = element.get(get_item, 0)#value set 0 because of C requirement
            if get_list_value:
                get_list_value = get_list_value.replace(" ", "_")
            get_list_values = get_list_values + (get_list_value,)
        return get_list_values

            
    def main(self):
        xml = self.get_xml()
        parsed_xml = self.parse_xml(xml)
        self.write_file_h_header()
        self.write_file_c_header()
        sub_structures = self.get_sub_structures(parsed_xml) #will be a list of partitions, partition memory, ect

        for sub_structure in sub_structures:
            if sub_structure:
                self.create_sub_structure_structs(sub_structure)

        self.write_to_file_h(self.externs_for_h_footer)
        self.write_file_h_footer()
        self.file_h.close()
        self.file_c.close()


if __name__ == "__main__":
    parse_xml = ParseXML()
    parse_xml.main()
