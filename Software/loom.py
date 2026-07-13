import json 

class Connection:
    def __init__(self,connector,contact):
        self.connector:Connector = connector
        self.contact:Contact = contact;

class Contact:
    def __init__(self):
        self.id:str = ""
        self.connections:list[Connection] = []
        self.pin_num:int = 0
        self.connector:Connector

    def __str__(self):
        s:str = ""
        s = s + self.id + " -> "
        for c in self.connections:
            s = s + c.connector.id + c.contact.id + ","
        l = list(s);
        l[len(s)-1] = '\n'
        s = "".join(l)
        return s

class Connector:
    def __init__(self):
        self.id:str = ""
        self.part_number:str = ""
        self.contacts:dict[str,Contact] = {}
        self.contact_count:int = 0
        self.width:int = 0
        self.height:int = 0
        self.shape:str = "" 


    def verify_part_number(self):
        match len(self.part_number):
            case 10:
                match self.part_number[:6]:
                    case "043025":
                        self.shape = "Molex"
                        self.height = 2
                        self.width = int(int(self.part_number[6:8])/self.height)
                        connector_pin_count = self.height*self.width
                        if(connector_pin_count < self.contact_count):
                            print("ERROR, contact_count more than connector pins")
                        elif(connector_pin_count > self.contact_count):
                            print("Warning, contact_count less than connector pins")
            case 9:
                match self.part_number[:5]:
                    case "43025":
                        self.shape = "Molex"
                        self.height = 2
                        self.width = int(int(self.part_number[5:7])/self.height)
                        connector_pin_count = self.height*self.width
                        if(connector_pin_count < self.contact_count):
                            print("ERROR, contact_count more than connector pins")
                        elif(connector_pin_count > self.contact_count):
                            print("Warning, contact_count less than connector pins")
            case 8:
                match self.part_number:
                    case "junction":
                        self.shape = "junction"
                        self.height = 1
                        self.width = 1
                        connector_pin_count = self.height*self.width
                        if(connector_pin_count < self.contact_count):
                            print("ERROR, contact_count more than connector pins")
                        elif(connector_pin_count > self.contact_count):
                            print("Warning, contact_count less than connector pins")
            case 7:
                match self.part_number:
                    case "534-216":
                        self.shape = "lug"
                        self.height = 1
                        self.width = 1
                        connector_pin_count = self.height*self.width
                        if(connector_pin_count < self.contact_count):
                            print("ERROR, contact_count more than connector pins")
                        elif(connector_pin_count > self.contact_count):
                            print("Warning, contact_count less than connector pins")



    def __str__(self):
        s:str = ""
        s = s + "\t id: " + self.id + "\n" 
        s = s + "\t part_number:" + self.part_number + "\n"
        s = s + "\t width:" + str(self.width) + "\n"
        s = s + "\t height:" + str(self.height) + "\n"
        s = s + "\t contact_count:" + str(self.contact_count) + "\n"
        s = s + "\t contacts: \n"
        for c in self.contacts.values():
            s = s +"\t\t" + str(c) + "\n"
        return s


class Loom:
    def __init__(self):
        self.part_number:str = ""
        self.revision:str = ""
        self.connectors:dict[str,Connector] = {}

    def __str__(self):
        s = ""
        s = s + self.part_number + ":rev-" + self.revision + "\n" 
        for c in self.connectors.values():
            s = s + str(c) + "\n"
            

        return s


def json_to_loom(path:str):
    ret_loom:Loom = Loom()
    with open(path, "r") as file:
        data = json.load(file)
    ret_loom.part_number = data["documents"]["default"]["meta"]["part_number"];
    for r in data["documents"]["default"]["revisions"]:
        ret_loom.revision = r["revision"]
    for con in data["connectors"]:
        connector:Connector = Connector();
        connector.id = con["id"]
        connector.part_number = con["part_number"]

        cont_suf_arr = con["contact_suffixes"]
        if len(cont_suf_arr) > 0:
            for cont_suf in cont_suf_arr:
                contact:Contact = Contact()
                contact.id = cont_suf
                contact.connector = connector
                connector.contacts[cont_suf] = contact

        connector.contact_count = len(connector.contacts)
        if connector.contact_count == 0:
            contact:Contact = Contact()
            contact.id = ""
            connector.contacts[connector.id] = contact
            connector.contact_count = len(connector.contacts)
        # if(connector.contact_count != len(connector.contacts)):
        #      print("Incorrect number of contacts specified !")
        #      print("USING len(connector.contacts) for calculations")
        #      connector.contact_count = len(connector.contacts)

        connector.verify_part_number()
        ret_loom.connectors[connector.id] = connector

    for j in data["junctions"]:
        connector:Connector = Connector();
        connector.id = j["id"]
        connector.part_number = "junction"
        contact:Contact = Contact()
        contact.id = ""
        connector.contacts[""] = contact
        connector.contact_count = len(connector.contacts)
        connector.verify_part_number()
        ret_loom.connectors[connector.id] = connector


    for con in data["connections"]:
        from_id:str = con["from_id"]
        to_id:str = con["to_id"]

        from_con:list[Connection] = []
        if '.' in from_id:
            from_sep = from_id.index('.')
            from_con = ret_loom.connectors[from_id[:from_sep]].contacts[from_id[from_sep:]].connections
        else:
            from_con = ret_loom.connectors[from_id].contacts[""].connections

        if '.' in to_id:
            to_sep = to_id.index('.')
            to_con = ret_loom.connectors[to_id[:to_sep]]
            from_con.append(Connection(to_con, to_con.contacts[to_id[to_sep:]]))
        else:
            to_con = ret_loom.connectors[to_id]
            from_con.append(Connection(to_con, Contact()))

            

    file.close()
    return ret_loom
