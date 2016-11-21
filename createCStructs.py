#!/usr/bin/python
import sys
import xmltodict # to parse the xml structure to an ordered dict
import json # to convert the ordered dict to a normal dict
from collections import OrderedDict

class ParseXML:


    def __init__(self):
        self.xml_schema = "schema.xml"
        self.file = open("hedder.c", "w")

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


    def print_declarations(self):
        partition_struct = "typedef struct{\n\t int partitionidentifier;\n\t char partitionname[32];\n\t char criticality[32];\n\t bool systempartion;\n\t char entrypoint[32];\n\t const queuing_port *queue_arr;\n\t const sampling_port *sample_arr;\n\t } partition;\n\n"
        q_ports_struct = "typedef struct {\n\t char portname[32];\n\t int maxmessagesize;\n\t char direction[32];\n\t int maxnbmessages;\n\t} queuing_port;\n\n"
        s_ports_struct = "typedef struct {\n\t char portname[32];\n\t int maxmessagesize;\n\t char direction[32];\n\t float refreshrateseconds;\n\t} sampling_port;\n\n"

        partition_memory_struct = "typedef struct{\n\t int partitionidentifier;\n\t char partitionname[32];\n\t const memory_requirements *memory_arr;\n\t} partition_memory;\n\n"
        memory_requirements_struct = "typedef struct {\n\t char type[32];\n\t int sizebytes;\n\t char access[32];\n\t char physicaladdress[32];\n\t} memory_requirements;\n\n"

        partition_schedule_struct = "typedef struct{\n\t int partitionidentifier;\n\t char partitionname[32];\n\t float peroidseconds;\n\t float perioddurationseconds;\n\t const window_schedule *window_arr;\n\t} partition_schedule;\n\n"
        window_schedule_struct = "typedef struct {\n\t int windowidentifier;\n\t float windowstartseconds;\n\t float windowdurationseconds;\n\t bool partitionperiodstart;\n\t} window_schedule;\n\n"
        declaration_list = [q_ports_struct, s_ports_struct, partition_struct, memory_requirements_struct, partition_memory_struct, window_schedule_struct, partition_memory_struct]

        
        return declaration_list


    def write_to_file(self, data):        
        self.file.write(data)


    #iterates through sub_structure to append string partition sructs in a list
    def create_sub_structure_structs(self, sub_structure):
        x = 0
        part_string = ""
        no_of_sub_elements = len(sub_structure)
        #print "number of sub_structure elements %s" % (no_of_sub_elements)
        
        while x < no_of_sub_elements:
            sub_element = sub_structure[x]
            sub_element_struct = self.construct_sub_element_struct(sub_element) #need to know if ports exist
            part_string = part_string + sub_element_struct
            x = x + 1


        partition = sub_element.get('Queuing_Port', None) 
        partition_memory = sub_element.get('Memory_Requirements', None)         
        partition_schedule = sub_element.get('Window_Schedule', None)         
        if partition:
            #print "const partition partitions[%s] = {%s};\n\n" % (no_of_sub_elements, part_string)
            complete_struct = "const partition partitions[%s] = {%s};\n\n" % (no_of_sub_elements, part_string)
            self.write_to_file(complete_struct)
        if partition_memory:
            #print "const partition_memory partition_memorys[%s] = {%s};\n\n" % (no_of_sub_elements, part_string)
            complete_struct = "const partition_memory partition_memorys[%s] = {%s};\n\n" % (no_of_sub_elements, part_string)
            self.write_to_file(complete_struct)
        if partition_schedule:
            #print "const partition_schedule partition_schedules[%s] = {%s};\n\n" % (no_of_sub_elements, part_string)
            complete_struct = "const partition_schedule partition_schedules[%s] = {%s};\n\n" % (no_of_sub_elements, part_string)
            self.write_to_file(complete_struct)


    def construct_sub_element_struct(self, sub_element):

        partition = sub_element.get('Queuing_Port', None) 
        partition_memory = sub_element.get('Memory_Requirements', None)         
        partition_schedule = sub_element.get('Window_Schedule', None)         
        sub_sub_element_struct = ""
        
        if partition: 
            sub_element_struct, element_name = self.partition_struct(sub_element)
            queuing_ports = sub_element.get("Queuing_Port", ())
            sampling_ports = sub_element.get("Sampling_Port", ())
            q_port_wrapper = ""
            s_port_wrapper = ""
            if queuing_ports: #vunerable to not having a queuing port, but having a sampling port
                queuing_ports = self.sub_sub_structure_validation(queuing_ports)
                q_port_string, no_of_q_ports = self.create_queuing_port_structs(queuing_ports)
                q_ports_wrapper = "const queuing_port queuep_%s[%s] = {%s};\n\n" % (element_name.replace (" ", "_"), no_of_q_ports, q_port_string)
                #print "\n" + q_ports_wrapper
                self.write_to_file(q_ports_wrapper)

            if sampling_ports:
                sampling_ports = self.sub_sub_structure_validation(sampling_ports)                
                s_port_string, no_of_s_ports = self.create_sampling_port_structs(sampling_ports)
                s_ports_wrapper = "const sampling_port samplep_%s[%s] = {%s};\n\n" % (element_name.replace (" ", "_"), no_of_s_ports, s_port_string)
                #print s_ports_wrapper
                self.write_to_file(s_ports_wrapper)
            partition = None
                

        elif partition_memory:
            sub_element_struct, element_name = self.partition_memory_struct(sub_element)
            memory_requirements = sub_element.get("Memory_Requirements", ())
            if memory_requirements:
                memory_requirements = self.sub_sub_structure_validation(memory_requirements)
                memory_requirements_string, no_of_memory_requirements = self.create_memory_requirements_structs(memory_requirements)
                memory_requirements_wrapper = "const memory_requirements memoryp_%s[%s] = {%s};\n\n" % (element_name.replace (" ", "_"), no_of_memory_requirements, memory_requirements_string)
                #print "\n" + memory_requirements_wrapper
            self.write_to_file(memory_requirements_wrapper)
            partition_memory = None


        elif partition_schedule:
            sub_element_struct, element_name = self.partition_schedule_struct(sub_element)
            window_schedule = sub_element.get("Window_Schedule", ())
            if window_schedule:
                window_schedule = self.sub_sub_structure_validation(window_schedule)
                window_schedule_string, no_of_window_schedules = self.create_window_schedules_structs(window_schedule)
                window_schedule_wrapper = "const window_schedule windowp_%s[%s] = {%s};\n\n" % (element_name.replace (" ", "_"), no_of_window_schedules, window_schedule_string)
                #print "\n" + window_schedule_wrapper
            self.write_to_file(window_schedule_wrapper)
            partition_schedule = None


        else:
            print "dunno what it is"
            print sub_element

        return sub_element_struct

    def partition_struct(self, sub_element):
        #had to use @ sign as it appears in the data for some unknown reason
        part_id = sub_element.get('@PartitionIdentifier', "nope") 
        name = sub_element.get('@PartitionName', "nope") 
        crit_level = sub_element.get('@Criticality', "nope") 
        sys_part = sub_element.get('@SystemPartition', "nope") 
        entry = sub_element.get('@EntryPoint', "nope") 
        queue_arr = "queuep_%s" % (name)
        sample_arr = "samplep_%s" % (name)
        partition_struct = "{ \n\t .partitionidentifier = %s,\n\t .partitionname = \"%s\",\n\t .criticality = \"%s\",\n\t .systempartion = %s,\n\t .entrypoint = \"%s\",\n\t .queue_arr = %s,\n\t .sample_arr = %s,\n\t}," % (part_id, name.replace (" ", "_"), crit_level, sys_part, entry, queue_arr.replace (" ", "_"), sample_arr.replace (" ", "_"))
        return partition_struct, name


    def partition_memory_struct(self, sub_element):
        part_id = sub_element.get('@PartitionIdentifier', "nope") 
        name = sub_element.get('@PartitionName', "nope") 
        memory_arr = "memoryp_%s" % (name)
        partition_memory_struct = "{ \n\t .partitionidentifier = %s,\n\t .partitionname = \"%s\",\n\t .memory_arr = %s,\n\t}," % (part_id, name.replace (" ", "_"), memory_arr.replace (" ", "_"))
        return partition_memory_struct, name

        
    def partition_schedule_struct(self, sub_element):
        part_id = sub_element.get('@PartitionIdentifier', "nope") 
        name = sub_element.get('@PartitionName', "nope") 
        period_seconds = sub_element.get('@PeriodSeconds', "nope") 
        period_duration_seconds = sub_element.get('@PeriodDurationSeconds', "nope") 
        window_arr = "windowp_%s" % (name)

        partition_schedule_struct = "{\n\t .partitionidentifier = %s,\n\t .partitionname = \"%s\",\n\t .peroidseconds = %s,\n\t .peroiddurationseconds = %s,\n\t .window_arr = %s,\n\t}," % (part_id, name.replace (" ", "_"), period_seconds, period_duration_seconds, window_arr.replace (" ", "_"))
        return partition_schedule_struct, name


    # ports ----------------------------------------------------------------------

    #takes in the ports and returns them in the correct structure. This is needed as ports can be in dictionaries or lists
    def sub_sub_structure_validation(self, sub_sub_structure):
        if sub_sub_structure:
            if type(sub_sub_structure) == dict:
                sub_sub_structure_list = []
                sub_sub_structure_list.append(sub_sub_structure)
                sub_sub_structure = sub_sub_structure_list

        return sub_sub_structure


    def create_queuing_port_structs(self, q_ports):
        x = 0
        q_port_string = ""
        no_of_ports = len(q_ports)
        while x < no_of_ports:
            port = q_ports[x]

            name = port.get('@PortName', "nope") 
            msg_size = port.get('@MaxMessageSize', "nope") 
            direction = port.get('@Direction', "nope") 
            max_msg = port.get('@MaxNbMessages', "nope") 

            q_port_struct = "{\n\t .portname = \"%s\",\n\t .maxmessagesize = %s,\n\t .direction = \"%s\",\n\t .maxnbmessages = %s,\n\t}," % (name.replace (" ", "_"), msg_size, direction, max_msg)
            q_port_string = q_port_string + q_port_struct
            x = x + 1
        return q_port_string, no_of_ports


    def create_sampling_port_structs(self, s_ports):
        x = 0
        s_port_string = ""
        no_of_ports = len(s_ports)
        while x < no_of_ports:
            port = s_ports[x]

            name = port.get('@PortName', "nope") 
            msg_size = port.get('@MaxMessageSize', "nope") 
            direction = port.get('@Direction', "nope") 
            r_rate = port.get('@RefreshRateSeconds', "nope") 

            s_port_struct = "{\n\t .portname = \"%s\",\n\t .maxmessagesize = %s,\n\t .direction = \"%s\",\n\t .refreshrateseconds = %s,\n\t}," % (name.replace (" ", "_"), msg_size, direction, r_rate)
            s_port_string = s_port_string + s_port_struct
            x = x + 1
        return s_port_string, no_of_ports


    def create_memory_requirements_structs(self, memory_requirements):
        x = 0
        memory_requirement_string = ""
        no_of_memory_requirements = len(memory_requirements)
        while x < no_of_memory_requirements:
            memory_requirement = memory_requirements[x]

            mem_type = memory_requirement.get('@Type', "nope") 
            size_bytes = memory_requirement.get('@SizeBytes', "nope") 
            access = memory_requirement.get('@Access', "nope") 
            physical_address = memory_requirement.get('@PhysicalAddress', "nope") 

            memory_requirement_struct = "{\n\t .type = \"%s\",\n\t .sizebytes = %s,\n\t .access = \"%s\",\n\t .physicaladdress = \"%s\",\n\t}," % (mem_type, size_bytes, access, physical_address)
            memory_requirement_string = memory_requirement_string + memory_requirement_struct
            x = x + 1
        return memory_requirement_string, no_of_memory_requirements


    def create_window_schedules_structs(self, window_schedules):
        x = 0
        window_schedule_string = ""
        no_of_window_schedules = len(window_schedules)
        while x < no_of_window_schedules:
            window_schedule = window_schedules[x]

            win_id = window_schedule.get('@WindowIdentifier', "nope") 
            win_start = window_schedule.get('@WindowStartSeconds', "nope") 
            win_duration = window_schedule.get('@WindowDurationSeconds', "nope") 
            part_period_start = window_schedule.get('@PartitionPeriodStart', "nope") 

            window_schedule_struct = "{\n\t .windowidentifier = %s,\n\t .windowstartseconds = %s,\n\t .windowdurationseconds = %s,\n\t .partitionperiodstart = %s,\n\t}," % (win_id, win_start, win_duration, part_period_start)
            window_schedule_string = window_schedule_string + window_schedule_struct

            x = x + 1
        return window_schedule_string, no_of_window_schedules


    def main(self):
        self.write_to_file("#include <stdbool.h>\n\n")
        xml = self.get_xml()
        parsed_xml = self.parse_xml(xml)
        declarations_list = self.print_declarations()

        x = 0
        while x < len(declarations_list):            
            self.write_to_file(declarations_list[x])
            x = x + 1

        sub_structures = self.get_sub_structures(parsed_xml) #will be a list of partitions, partition memory, ect

        no_of_sub_structures = len(sub_structures)
        x = 0
        while x < no_of_sub_structures:
            sub_structure = sub_structures[x]
            self.create_sub_structure_structs(sub_structure)
            x = x + 1

        self.file.close()


if __name__ == "__main__":
    parse_xml = ParseXML()
    parse_xml.main()

    
    
