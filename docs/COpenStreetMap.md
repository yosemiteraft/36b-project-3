# COpenStreetMap

COpenStreetMap is a concrete class that parses OpenStreetMap XML data. This includes nodes and ways.

## Constructor

COpenStreetMap(std::shared_ptr<CXMLReader> src);

Creates a COpenStreetMap instance using an XML reader to parse OSM XML data (nodes, ways, and their attributes).

**Parameters:**

* src: XML reader containing OSM XML data

**Example usage:**

auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n                                                            "
                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\"/>\n"
                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\"/>\n"
                                                            "  <way id=\"1000\">\n"
                                                            "    <nd ref=\"1\"/>\n"
                                                            "    <nd ref=\"2\"/>\n"
                                                            "  </way>\n"
                                                        "</osm>"
                                                        );
auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
COpenStreetMap OpenStreetMap(OSMReader);

## Member Functions

### NodeCount

std::size_t NodeCount() const noexcept override;

Returns the total number of parsed OSM nodes in the system.

**Return types:**

* std::size_t: Number of nodes parsed from the OSM XML data

**Example usage:**

auto OSMReader = std::make_shared<CXMLReader>(OSMSource);
COpenStreetMap OpenStreetMap(OSMReader);
std::size_t NumNodes = OpenStreetMap.NodeCount();

### WayCount

std::size_t WayCount() const noexcept override;

Returns the total number of parsed OSM ways in the system.

**Return types:**

* std::size_t: Number of ways parsed from the OSM XML data

**Example usage:**

auto OSMReader = std::make_shared<CXMLReader>(OSMSource);
COpenStreetMap OpenStreetMap(OSMReader);
std::size_t NumWays = OpenStreetMap.WayCount(); 

### NodeByIndex

std::shared_ptrCStreetMap::SNode NodeByIndex(std::size_t index) const noexcept override;

Accesses a parsed OSM node by its index

**Parameters:**

* index: Index of the node to access

**Return types:**

* std::shared_ptr<CStreetMap::SNode>: Valid node pointer if index is within bounds
* nullptr: Invalid index provided (out of bounds or negative)

**Example usage:**

auto OSMReader = std::make_shared<CXMLReader>(OSMSource);
COpenStreetMap OpenStreetMap(OSMReader);
auto NodeIndex0 = OpenStreetMap.NodeByIndex(0);
EXPECT_EQ(NodeIndex0->ID(), 1);

### NodeByID

std::shared_ptrCStreetMap::SNode NodeByID(CStreetMap::TNodeID id) const noexcept override;

Accesses a parsed OSM node by its unique ID (parsed from the id attribute of the <node> element).

**Parameters:**

id: Unique TNodeID associated with a particular node

**Return types:**

* std::shared_ptr<CStreetMap::SNode>: Valid node pointer if ID exists in parsed data
* nullptr: Invalid/no ID provided

**Example usage:**

auto OSMReader = std::make_shared<CXMLReader>(OSMSource);
COpenStreetMap OpenStreetMap(OSMReader);
auto NodeID1 = OpenStreetMap.NodeByID(1);
EXPECT_EQ(NodeID1->ID(), 1);

### WayByIndex

std::shared_ptrCStreetMap::SWay WayByIndex(std::size_t index) const noexcept override;

Accesses a parsed OSM way by its index (ordered as parsed from the XML data).

**Parameters:**

* index: Index of the way to access (0-based)

**Return types:**

* std::shared_ptr<CStreetMap::SWay>: Valid way pointer if index is within bounds
* nullptr: Invalid index provided (out of bounds or negative)

**Example usage:**

auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
COpenStreetMap OpenStreetMap(OSMReader);
auto WayIndex0 = OpenStreetMap.WayByIndex(0);
EXPECT_EQ(WayIndex0->ID(), 1000);

### WayByID

std::shared_ptrCStreetMap::SWay WayByID(CStreetMap::TWayID id) const noexcept override;

Accesses a parsed OSM way by its unique ID (parsed from the id attribute of the <way> element).

**Parameters:**

* id: TWayID associated with a particular way

**Return types:**

* std::shared_ptr<CStreetMap::SWay>: Valid way pointer if ID exists in parsed data
* nullptr: Invalid/no ID provided

**Example usage:**

auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
COpenStreetMap OpenStreetMap(OSMReader);
auto WayID1000 = OpenStreetMap.WayByID(1000);
EXPECT_EQ(WayID1000->ID(), 1000);