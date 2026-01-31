# CXMLReader

CXMLReader reads in from a source and parses into start, character data, and end element entities.

## Constructor

CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src)

Sets up the parser with the XML data source, making an instance of SImplementation

**Parameters:**

* src: shared pointer to an instance of CDataSource. This is the XML text that XMLReader will read from

**Example usage:**

    std::shared_ptr <CDataSource> src = std::make_shared<CDataSource>();
    CXMLReader(src);

## SImplementation Struct Functions

### ExpatStartElement

static void ExpatStartElement(void *data, const XML_Char *name, const XML_Char **attrs)

creates a StartElement entity with the tag’s name and attributes, then adds it to the parser’s entity queue (DQueue) to be used by ReadEntity() later

**Parameters:**

* data: pointer to an instance of SImplementation (which holds the entity queue)
* name: name of the xml start tag
* attrs: array to store key-value pairs of the xml tag's attributes

**Return types:**

* none, but adds entities to DQueue

**Example usage:**
std::shared_ptr<CStringDataSource> src = std::make_shared<CStringDataSource>("<tag attr1=\"val1\"></tag>");
CXMLReader reader(src);
SXMLEntity entity;
reader.ReadEntity(entity, false);

### ExpatEndElement

static void ExpatEndElement(void *data, const XML_Char *name)

creates an EndElement entity with the tag's name and adds it to DQueue

**Parameters:**

* data: pointer to an instance of SImplementation (which holds the entity queue)
* name: name of the xml end tag

**Return types:**

* none, but adds entities to DQueue

**Example usage:**

std::shared_ptr<CStringDataSource> src = std::make_shared<CStringDataSource>("<tag attr1=\"val1\"></tag>");
CXMLReader reader(src);
SXMLEntity entity;
reader.ReadEntity(entity, false);

### ExpatCharacterData

static void ExpatCharacterData(void *data, const XML_Char *s, int len)

usage:

**Parameters:**

* data: pointer to an instance of SImplementation (which holds the entity queue)
* s: pointer to the parsed text in between the start and end tags
* len: number of characters in the s buffer (ensures all characters are read)

**Return types:**

* none, but adds entities to DQueue

**Example usage:**
std::shared_ptr<CStringDataSource> src = std::make_shared<CStringDataSource>("<tag>CData</tag>");
CXMLReader reader(src);
SXMLEntity entity;
reader.ReadEntity(entity, true);


### End

bool End() const

Checks if it is the end of data source (no more to XML text read)

**Parameters:**

* none

**Return types:**

* true: all entities have been read from the XML
* false: still more to read from data source

**Example usage:**
std::shared_ptr<CStringDataSource> src = std::make_shared<CStringDataSource>("<tag>CData</tag>");
CXMLReader reader(src);
SXMLEntity entity;
if(read.End()){
    std::cout<<"End reached"<<std::endl;
}

### ReadEntity

bool ReadEntity(SXMLEntity &entity, bool skipcdata)

Read in an XML entity from DQueue (queue of parsed xml elements), skips character data if skpcdata is true

**Parameters:**

* entity: reference to SXML object to store the next XML entity (start, end, character data text)
* skipcdata: boolean to tell if character data should be skipped or not

**Return types:**

* true: valid entity succesfully read
* false: nothing to read in (queue and source empty), entity not read

**Example usage:**
reader.ReadEntity(entity, true)

### RefillQueue

void RefillQueue()

refills queue if queue is empty and d source still has xml text

**Return types:**
* nothing, but adds to DQueue

**Example usage:**
std::string longCData(300, '?');
std::string longstr("<tag>" + longCData + "</tag>");
std::shared_ptr<CStringDataSource> longsrc = std::make_shared<CStringDataSource>(longstr);
CXMLReader reader(longsrc);
SXMLEntity entity;
