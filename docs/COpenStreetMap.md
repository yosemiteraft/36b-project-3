# COpenStreetMap

COpenStreetMap is a concrete class that parses OpenStreetMap XML data. It extracts geographic nodes (points with latitude/longitude) and ways (paths composed of nodes) from XML input and provides fast access to parsed data via index or ID lookup.

## Constructor

COpenStreetMap(std::shared_ptr<CXMLReader> src);

Creates a COpenStreetMap instance using an XML reader to parse OSM XML data (nodes, ways, and their attributes).

**Parameters:**

* src: XML reader containing OSM XML data (root <osm> tag with <node> and <way> child elements)

**Example usage:**
// Create a string-based XML data source with sample OSM data
auto OSMSource = std::make_shared<CStringDataSource>(  
    "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
    "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\">\n"
    "    <tag k=\"highway\" v=\"motorway_junction\"/>\n"
    "  </node>\n"
    "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\"/>\n"
    "  <way id=\"1000\">\n"
    "    <nd ref=\"1\"/>\n"
    "    <nd ref=\"2\"/>\n"
    "  </way>\n"
    "</osm>"
);
// Create XML reader for the data source
auto OSMReader = std::make_shared<CXMLReader>(OSMSource);
// Initialize COpenStreetMap with the XML reader
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
std::size_t NumNodes = OpenStreetMap.NodeCount(); // Returns 2 for the sample data

### WayCount

std::size_t WayCount() const noexcept override;

Returns the total number of parsed OSM ways in the system.

**Return types:**

* std::size_t: Number of ways parsed from the OSM XML data

**Example usage:**

auto OSMReader = std::make_shared<CXMLReader>(OSMSource);
COpenStreetMap OpenStreetMap(OSMReader);
std::size_t NumWays = OpenStreetMap.WayCount(); // Returns 1 for the sample data

### NodeByIndex

std::shared_ptrCStreetMap::SNode NodeByIndex(std::size_t index) const noexcept override;

Accesses a parsed OSM node by its index (ordered as parsed from the XML data).

**Parameters:**

* index: Index of the node to access (0-based)

**Return types:**

* std::shared_ptr<CStreetMap::SNode>: Valid node pointer if index is within bounds
* nullptr: Invalid index provided (out of bounds or negative)

**Example usage:**
auto OSMReader = std::make_shared<CXMLReader>(OSMSource);
COpenStreetMap OpenStreetMap(OSMReader);
auto Node = OpenStreetMap.NodeByIndex(0); // Returns node with ID 1 (sample data)
if (Node) {
    std::cout << "Node ID: " << Node->ID() << std::endl; // Prints "Node ID: 1"
}

### NodeByID

std::shared_ptrCStreetMap::SNode NodeByID(CStreetMap::TNodeID id) const noexcept override;

Accesses a parsed OSM node by its unique ID (parsed from the id attribute of the <node> element).

**Parameters:**

id: Unique TNodeID (uint64_t) associated with a particular node

**Return types:**

* std::shared_ptr<CStreetMap::SNode>: Valid node pointer if ID exists in parsed data
* nullptr: Invalid/non-existent ID provided

**Example usage:**

auto OSMReader = std::make_shared<CXMLReader>(OSMSource);
COpenStreetMap OpenStreetMap(OSMReader);
auto Node = OpenStreetMap.NodeByID(2); // Returns node with ID 2 (sample data)
if (Node) {
    std::cout << "Node Latitude: " << Node->Location().DLatitude << std::endl; // Prints "Node Latitude: 38.5"
}

### WayByIndex

std::shared_ptrCStreetMap::SWay WayByIndex(std::size_t index) const noexcept override;

Accesses a parsed OSM way by its index (ordered as parsed from the XML data).

**Parameters:**

* index: Index of the way to access (0-based)

**Return types:**

* std::shared_ptr<CStreetMap::SWay>: Valid way pointer if index is within bounds
* nullptr: Invalid index provided (out of bounds or negative)

**Example usage:**

auto OSMReader = std::make_shared<CXMLReader>(OSMSource);
COpenStreetMap OpenStreetMap(OSMReader);
auto Way = OpenStreetMap.WayByIndex(0); // Returns way with ID 1000 (sample data)
if (Way) {
    std::cout << "Way Node Count: " << Way->NodeCount() << std::endl; // Prints "Way Node Count: 2"
}

### WayByID

std::shared_ptrCStreetMap::SWay WayByID(CStreetMap::TWayID id) const noexcept override;

Accesses a parsed OSM way by its unique ID (parsed from the id attribute of the <way> element).

**Parameters:**

* id: Unique TWayID (uint64_t) associated with a particular way

**Return types:**

* std::shared_ptr<CStreetMap::SWay>: Valid way pointer if ID exists in parsed data
* nullptr: Invalid/non-existent ID provided

**Example usage:**

auto OSMReader = std::make_shared<CXMLReader>(OSMSource);
COpenStreetMap OpenStreetMap(OSMReader);
auto Way = OpenStreetMap.WayByID(1000); // Returns way with ID 1000 (sample data)
if (Way) {
    std::cout << "First Node ID in Way: " << Way->GetNodeID(0) << std::endl; // Prints "First Node ID in Way: 1"
}
