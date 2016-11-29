#!/usr/bin/python
import sys
import xmltodict # to parse the xml structure to an ordered dict
import json # to convert the ordered dict to a normal dict
from collections import OrderedDict

class ParseXML:


    def __init__(self):
        self.xml_schema = "schema.xml"
        self.file_h = open("xml_data.h", "w")
        self.file_c = open("xml_data.c", "w")


    def get_xml(self):
        print "reading in xml from file %s" % (self.xml_schema)
        with open (self.xml_schema, "r") as xmlfile:
            xml=xmlfile.read()
        xmlfile.closed
        #print "xml file contents: %s" % (xml)
        return xml


    def parse_xml(self, xml):
        print "parsing xml file contents"
        parsed_xml = json.loads(json.dumps(xmltodict.parse(xml)))
        #print "parsed xml: %s" % (parsedxml)
        return parsed_xml


    def get_sub_structures(self, parsed_xml): #return a list of these?
        partitions = parsed_xml.get('ARINC_653_Module').get('Partition')
        partition_memory = parsed_xml.get('ARINC_653_Module').get('Partition_Memory')
        partition_schedule = parsed_xml.get('ARINC_653_Module').get('Module_Schedule').get('Partition_Schedule')
        sub_structures = [partitions, partition_memory, partition_schedule]
        return sub_structures


    def write_to_file_h(self, data):        
        self.file_h.write(data)


    def write_to_file_c(self, data):        
        self.file_c.write(data)


    def write_file_h_header(self):
        self.write_to_file_h(
"""#ifndef XML_DATA_H
#define XML_DATA_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include \"APEX/apex_queuing.h\"
#include \"APEX/apex_sampling.h\"
#include \"APEX/apex_partition.h\"

#include \"circular_buffer.h\"

/*
 * This macro simplifies the generation of message buffers.
 * It sets the maximum message size, maximum message number
 * and the buffer to hold them.
 */
#define MESSAGE_BUFFER(nb_message, message_size) \
    .MAX_MESSAGE_SIZE = message_size, \
    .MAX_NB_MESSAGE = nb_message, \
    .buffer = (uint8_t [(message_size * nb_message) + \
        (message_size * sizeof(size_t))]) {0}, \
    .circ_buf = {0, 0, 0, (message_size * nb_message) + \
        (message_size * sizeof(size_t))}

""")


    def write_file_h_footer(self):
        self.write_to_file_h(
"""#endif""")


    def write_file_c_header(self):
        self.write_to_file_c("""#include "xml_data.h"

""")


    def write_file_c_footer(self):
        x = "this function is not used currently"
        #self.write_to_file_c(""" """)


    def write_file_h_declarations(self):
        partition_struct = """
    typedef struct{
        int partitionidentifier;
        char partitionname[32];
        char criticality[32];
        bool systempartion;
        void (*entrypoint)(void);
        const queuing_port *queue_arr;
        const sampling_port *sample_arr;
    } partition;
        """
        q_ports_struct = """
    typedef struct {
        char portname[32];
        int maxmessagesize;
        char direction[32];
        int maxnbmessages;
    } queuing_port;
        """
        s_ports_struct = """
    typedef struct {
        char portname[32];
        int maxmessagesize;
        char direction[32];
        float refreshrateseconds;
    } sampling_port;
        """

        partition_memory_struct = """
    typedef struct{
        int partitionidentifier;
        char partitionname[32];
        const memory_requirements *memory_arr;
    } partition_memory;
        """
        memory_requirements_struct = """
    typedef struct {
        char type[32];
        int sizebytes;
        char access[32];
        char physicaladdress[32];
    } memory_requirements;
        """

        partition_schedule_struct = """
    typedef struct{
        int partitionidentifier;
        char partitionname[32];
        float peroidseconds;
        float perioddurationseconds;
        const window_schedule *window_arr;
    } partition_schedule;
        """
        window_schedule_struct = """
    typedef struct {
        int windowidentifier;
        float windowstartseconds;
        float windowdurationseconds;
        bool partitionperiodstart;
    } window_schedule;
        """

        declaration_list = [q_ports_struct, s_ports_struct, partition_struct, 
                            memory_requirements_struct, partition_memory_struct, 
                            window_schedule_struct, partition_schedule_struct]        
        for n in declaration_list:
            self.write_to_file_h(n)


    def create_sub_structure_structs(self, sub_structure):
        structs_string = ""
        no_of_sub_elements = len(sub_structure)
        
        for sub_element in sub_structure:
            sub_element_struct = self.construct_sub_element_struct(sub_element)
            structs_string = structs_string + sub_element_struct

        partition = sub_element.get('Queuing_Port', None) 
        partition_memory = sub_element.get('Memory_Requirements', None)         
        partition_schedule = sub_element.get('Window_Schedule', None)         
        if partition:
            complete_struct = "const partition partitions[%s] = {%s};\n\n" % (no_of_sub_elements, structs_string)
            self.write_to_file_c(complete_struct)
        if partition_memory:
            complete_struct = "const partition_memory partition_memorys[%s] = {%s};\n\n" % (no_of_sub_elements, structs_string)
            self.write_to_file_c(complete_struct)
        if partition_schedule:
            complete_struct = "const partition_schedule partition_schedules[%s] = {%s};\n\n" % (no_of_sub_elements, structs_string)
            self.write_to_file_c(complete_struct)


    def construct_sub_element_struct(self, sub_element):
        queuing_ports = sub_element.get("Queuing_Port", ())
        sampling_ports = sub_element.get("Sampling_Port", ())
        memory_requirements = sub_element.get("Memory_Requirements", ())
        window_schedule = sub_element.get("Window_Schedule", ())

        
        if queuing_ports:
            sub_element_struct, sub_element_name = self.partition_struct(sub_element, sampling_ports)

            structs_string = ""
            get_list = ['@PortName', '@MaxMessageSize', '@Direction', '@MaxNbMessages']
            no_of_ports = len(queuing_ports)
            queuing_ports = self.sub_sub_structure_validation(queuing_ports)
            for port in queuing_ports:
                get_list_tuple = self.return_get_tuple(port, get_list)
                q_port_struct = """{{
    .portname = \"{}\",
    .maxmessagesize = {},
    .direction = {},
    .maxnbmessages = {},
    }},""".format(*get_list_tuple)
                structs_string = structs_string + q_port_struct
            q_ports_wrapper = "const queuing_port queuep_%s[%s] = {%s};\n\n" % (sub_element_name, no_of_ports, structs_string)
            self.write_to_file_c(q_ports_wrapper)

            if sampling_ports:
                structs_string = ""
                get_list = ['@PortName', '@MaxMessageSize', '@Direction', '@RefreshRateSeconds']
                no_of_ports = len(sampling_ports)
                sampling_ports = self.sub_sub_structure_validation(sampling_ports)                
                #s_port_string, no_of_s_ports = self.create_sampling_port_structs(sampling_ports)
                for port in sampling_ports:
                    get_list_tuple = self.return_get_tuple(port, get_list)
                    s_port_struct = """{{
    .portname = \"{}\",
    .maxmessagesize = {},
    .direction = \"{}\",
    .refreshrateseconds = {},
    }},""".format(*get_list_tuple)
                    structs_string = structs_string + s_port_struct
                s_ports_wrapper = "const sampling_port samplep_%s[%s] = {%s};\n\n" % (sub_element_name, no_of_ports, structs_string)
                self.write_to_file_c(s_ports_wrapper)


        elif memory_requirements:
            sub_element_struct, sub_element_name = self.partition_memory_struct(sub_element)
            structs_string = ""
            get_list = ['@Type', '@SizeBytes', '@Access', '@PhysicalAddress']
            no_of_mem_req = len(memory_requirements)
            memory_requirements = self.sub_sub_structure_validation(memory_requirements)
            #mem_requirements_string, no_of_mem_requirements = self.create_memory_requirements_structs(mem_requirements)
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
            no_of_win_sch = len(memory_requirements)
            window_schedule = self.sub_sub_structure_validation(window_schedule)
            #win_schedule_string, no_of_win_schedules = self.create_window_schedules_structs(win_schedule)
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

        else:
            print "unknown sub_element: %s" % (sub_element)

        return sub_element_struct


    def partition_struct(self, sub_element, sampling_ports):
        #had to use @ sign as it appears in the data for some unknown reason
        part_id = sub_element.get('@PartitionIdentifier', None) 
        name = sub_element.get('@PartitionName', None).replace (" ", "_")
        crit_level = sub_element.get('@Criticality', None) 
        sys_part = sub_element.get('@SystemPartition', None) 
        entry = "&" + sub_element.get('@EntryPoint', None) 
        queue_arr = "queuep_%s" % (name).replace (" ", "_")
        sample_arr = "samplep_%s" % (name).replace (" ", "_")
        if not sampling_ports:
            sample_arr = 0
        partition_struct = """{ 
    .partitionidentifier = %s,
    .partitionname = \"%s\",
    .criticality = \"%s\",
    .systempartion = %s,
    .entrypoint = %s,
    .queue_arr = %s,
    .sample_arr = %s,
    },""" % (part_id, name, crit_level, sys_part, entry, queue_arr, sample_arr)

        #this write the entry point to the .h file as it is a function needed to be defined
        self.write_to_file_h("""
void %s(void);
""" % (entry))
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


    def sub_sub_structure_validation(self, sub_sub_structure):
        if sub_sub_structure:
            if type(sub_sub_structure) == dict:
                sub_sub_structure_list = []
                sub_sub_structure_list.append(sub_sub_structure)
                sub_sub_structure = sub_sub_structure_list
        return sub_sub_structure


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
        self.write_file_h_declarations()
        self.write_file_c_header()
        sub_structures = self.get_sub_structures(parsed_xml) #will be a list of partitions, partition memory, ect

        for sub_structure in sub_structures:
            self.create_sub_structure_structs(sub_structure)

        self.write_file_h_footer()
        self.file_h.close()
        self.file_c.close()


if __name__ == "__main__":
    parse_xml = ParseXML()
    parse_xml.main()
