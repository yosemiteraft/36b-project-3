# CStreetMap

CStreetMap is the abstract base class that is the interface for COpenStreetMap, consisting of pure virtual functions that are implemented by COpenStreetMap.

## Failure Return

* InvalidNodeID: max value of TNodeID, used to specify invalid node IDs
* InvalidWayID: max value of TWayID, used to specify invalid way IDs
* nullptr: returned by node/way methods for invalid indexes/IDs
* Empty string (""): returned by attribute methods for invalid keys/indexes

## Nested Structs

### SLocation

SLocation is a struct that represents a coordinate (latitude/longitude) for map nodes.

**Constructors and Operators:**

* SLocation(): default constructor (initializes lat/long to default values)
* SLocation(const CStreetMap::SLocation &loc): copy constructor (copies lat/long from another SLocation)
* SLocation(double latitude, double longitude): constructor (explicitly defines lat/long)
* bool operator==(const SLocation &loc) const: returns if two SLocation objects have same lat/long values

### SNode

SNode is an abstract struct that represents a node/point on the street map.

**Virtual Member Functions:**

* TNodeID ID() const noexcept: returns the node ID
* SLocation Location() const noexcept: returns the lat/long of the node
* std::size_t AttributeCount() const noexcept: returns the number of key-value attributes of the node
* std::string GetAttributeKey(std::size_t index) const noexcept: returns the attribute key at the given index; empty string if index is invalid
* bool HasAttribute(const std::string &key) const noexcept: returns if the the specified key has an attribute
* std::string GetAttribute(const std::string &key) const noexcept: returns the value of the attribute with the specified key; returns empty string if key is not found

### SWay

SWay is an abstract struct that represents a way (path/road) on the street map, composed of a sequence of nodes.

**Virtual Member Functions:**

* TWayID ID() const noexcept: returns the ID of the way
* std::size_t NodeCount() const noexcept: returns the number of nodes in the way
* TNodeID GetNodeID(std::size_t index) const noexcept: returns the node ID at the index or InvalidNodeID if index is invalid
* std::size_t AttributeCount() const noexcept: returns the number of key-value attributes associated with the way
* std::string GetAttributeKey(std::size_t index) const noexcept: returns the attribute key at the index or empty string if invalid index
* bool HasAttribute(const std::string &key) const noexcept: returns if the key has an attribute
* std::string GetAttribute(const std::string &key) const noexcept: returns the value of the attribute at the key, or empty string if key is not found

## Member Functions

### NodeCount

virtual std::size_t NodeCount() const noexcept = 0;

Returns the total number of nodes in the street map system.

**Return types:**

* std::size_t: Number of nodes in the system

### WayCount

virtual std::size_t WayCount() const noexcept = 0;

Returns the total number of ways in the street map system.

**Return types:**

* std::size_t: Number of ways in the system

### NodeByIndex

virtual std::shared_ptr<SNode> NodeByIndex(std::size_t index) const noexcept = 0;

Accesses a map node by its index.

**Parameters:**

* index: Index of the node to access

**Return types:**

* std::shared_ptr<SNode>: Valid node pointer if index is provided
* nullptr: Invalid index provided

### NodeByID

virtual std::shared_ptr<SNode> NodeByID(TNodeID id) const noexcept = 0;

Accesses a map node by its unique node ID.

**Parameters:**

* id: Unique ID associated with a particular node

**Return types:**

* std::shared_ptr<SNode>: Valid node pointer if ID is provided
* nullptr: Invalid/non-existent ID provided

### WayByIndex

virtual std::shared_ptr<SWay> WayByIndex(std::size_t index) const noexcept = 0;

Accesses a map way by its index.

**Parameters:**

* index: Index of the way to access

**Return types:**

* std::shared_ptr<SWay>: Valid way pointer if index is provided
* nullptr: Invalid index provided

### WayByID

virtual std::shared_ptr<SWay> WayByID(TWayID id) const noexcept = 0;

Accesses a map way by its unique way ID.

**Parameters:**

* id: Unique ID associated with a particular way

**Return types:**

* std::shared_ptr<SWay>: Valid way pointer if ID is provided
* nullptr: Invalid/non-existent ID provided