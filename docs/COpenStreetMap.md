# COpenStreetMap

COpenStreetMap is a concrete class that parses OpenStreetMap XML data and provides access to parsed nodes and ways.

## Failure Return

* InvalidNodeID: max value of TNodeID, used to specify invalid node IDs
* nullptr: returned by node and way accessor methods for invalid indexes or IDs
* Empty string (""): returned by attribute accessor methods for invalid keys/indexes

## Nested Structs

### SNode

SNode is a concrete struct that represents an OSM node (latitude/longitude and optional attributes).

**Member Functions (Override Base Class):**

* TNodeID ID() const noexcept: returns unique node ID
* CStreetMap::SLocation Location() const noexcept: returns node latitude/longitude
* std::size_t AttributeCount() const noexcept: returns number of node attributes (<tag> elements)
* std::string GetAttributeKey(std::size_t index) const noexcept: returns attribute key at index (empty string if invalid)
* bool HasAttribute(const std::string &key) const noexcept: returns true if node has the attribute key, false otherwise
* std::string GetAttribute(const std::string &key) const noexcept: returns attribute value for key (empty string if key not found)

**Helper Functions:**

* void AddAttribute(const std::string &key, const std::string &value): adds attribute key-value pair to node (from OSM <tag>)

### SWay

SWay is a concrete struct (inherits from CStreetMap::SWay) that represents an OSM way (a collection of nodes forming a path, e.g., roads/paths, with optional attributes).

**Member Functions (Override Base Class):**

* TWayID ID() const noexcept: returns unique way ID
* std::size_t NodeCount() const noexcept: returns number of nodes in way (from <nd ref="X">)
* CStreetMap::TNodeID GetNodeID(std::size_t index) const noexcept: returns node ID at index (InvalidNodeID if invalid)
* std::size_t AttributeCount() const noexcept: returns number of way attributes (<tag> elements)
* std::string GetAttributeKey(std::size_t index) const noexcept: returns attribute key at index (empty string if invalid)
* bool HasAttribute(const std::string &key) const noexcept: returns true if way has attribute key, false otherwise
* std::string GetAttribute(const std::string &key) const noexcept: returns attribute value for key (empty string if key not found)

**Helper Functions:**

* void AddAttribute(const std::string &key, const std::string &value): adds attribute key-value pair to way (from OSM <tag>)
* void AddNodeID(TNodeID nodeid): adds node ID to way (from OSM <nd ref="X">)

### SImplementation

**Helper Parsing Functions:**

* bool FindStartTag: reads XML until specified start tag (e.g., <osm>) is found (returns true if found)
* bool FindEndTag: reads XML until specified end tag (e.g., </osm>) is found (returns true if found)
* bool ParseOSM: main parser – parses <osm> root and calls ParseNode()/ParseWay()
* bool ParseNode: parses <node> (ID/location/attributes) and stores in node collections
* bool ParseWay: parses <way> (ID/nodes/attributes) and stores in way collections

## Member Functions

### NodeCount

std::size_t NodeCount() const noexcept;

Returns the total number of parsed OSM nodes in the system.

**Return types:**

* std::size_t: Number of nodes in the system


### WayCount

std::size_t WayCount() const noexcept;

Returns the total number of parsed OSM ways in the system.

**Return types:**

* std::size_t: Number of ways in the system

### NodeByIndex

std::shared_ptrCStreetMap::SNode NodeByIndex(std::size_t index) const noexcept;

Accesses an OSM node by its index (ordered as parsed from XML).

**Parameters:**

* index: Index of the node to access

**Return types:**

* std::shared_ptr<CStreetMap::SNode>: Valid node pointer if index is within bounds
* nullptr: Invalid index provided (out of bounds or negative)

### NodeByID

std::shared_ptrCStreetMap::SNode NodeByID(TNodeID id) const noexcept;

Accesses an OSM node by its unique ID (parsed from the id attribute of the <node> element).

**Parameters:**

* id: Unique ID associated with a particular node

**Return types:**

* std::shared_ptr<CStreetMap::SNode>: Valid node pointer if ID exists
* nullptr: Invalid/non-existent ID provided

### WayByIndex

std::shared_ptrCStreetMap::SWay WayByIndex(std::size_t index) const noexcept;

Accesses an OSM way by its index (ordered as parsed from XML).

**Parameters:**

* index: Index of the way to access

**Return types:**

* std::shared_ptr<CStreetMap::SWay>: Valid way pointer if index is within bounds
* nullptr: Invalid index provided (out of bounds or negative)

### WayByID

std::shared_ptrCStreetMap::SWay WayByID(TWayID id) const noexcept;

Accesses an OSM way by its unique ID (parsed from the id attribute of the <way> element).

**Parameters:**

* id: Unique ID associated with a particular way

**Return types:**

* std::shared_ptr<CStreetMap::SWay>: Valid way pointer if ID exists
* nullptr: Invalid/non-existent ID provided
