# CBusSystem

CBusSystem is the abstract base class that is the interface for CXMLBusSystem, consisting of pure virtual functions that are implemented by CXMLBusSystem.

## Failure Return

* InvalidStopID: maximum value of TStopID, used to specify invalid stop IDs

## Nested Structs

### SStop

SStop is an abstract struct that represents a bus stop.

**Virtual Member Functions:**

* TStopID ID() const noexcept: returns stop ID of a stop
* CStreetMap::TNodeID NodeID() const noexcept: returns node ID of a stop
* std::string Description() const noexcept: returns description of a stop
* std::string Description(const std::string &description) noexcept: sets a new description of a stop and returns the new description

### SRoute

SRoute is an abstract struct that represents a bus route.

**Virtual Member Functions:**

* std::string Name() const noexcept: returns name of a route
* std::size_t StopCount() const noexcept: returns number of stops in a route
* TStopID GetStopID(std::size_t index) const noexcept: returns stop ID at a specified index, or InvalidStopID if index is invalid

### SPath

SPath is an abstract struct that represents a path between two nodes.

**Virtual Member Functions:**

* CStreetMap::TNodeID StartNodeID() const noexcept: returns node ID of start node of path
* CStreetMap::TNodeID EndNodeID() const noexcept: returns node ID of end node of path
* std::size_t NodeCount() const noexcept: returns number of nodes of a path
* CStreetMap::TNodeID GetNodeID(std::size_t index) const noexcept: returns node ID at specified index, or 0 if index is invalid

## Member Functions

### StopCount

virtual std::size_t StopCount() const noexcept = 0;

Returns total number of bus stops in system.

**Return types:**

* std::size_t: number of stops in the system

### RouteCount

virtual std::size_t RouteCount() const noexcept = 0;

Returns total number of bus routes in system.

**Return types:**

* std::size_t: number of routes in system

### StopByIndex

virtual std::shared_ptr<SStop> StopByIndex(std::size_t index) const noexcept = 0;

Accesses a bus stop by its index.

**Parameters:**

* index: index of stop to access

**Return types:**

* shared_ptr to stop: if valid index provided
* nullptr: invalid index provided

### StopByID

virtual std::shared_ptr<SStop> StopByID(TStopID id) const noexcept = 0;

Accesses a bus stop by its stop ID.

**Parameters:**

* id: a unique ID associated with a particular stop

**Return types:**

* shared_ptr to stop: if valid ID provided
* nullptr: invalid ID provided

### RouteByIndex

virtual std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const noexcept = 0;

Accesses a bus route by its index.

**Parameters:**

* index: index of bus route to access

**Return types:**

* shared_ptr to route: if valid index provided
* nullptr: invalid index provided

### RouteByName

virtual std::shared_ptr<SRoute> RouteByName(const std::string &name) const noexcept = 0;

Accesses a bus route by its name.

**Parameters:**

* name: name of bus route to access

**Return types:**

* shared_ptr to route: if valid, existing name provided
* nullptr: if invalid name provided

### PathByStopIDs

virtual std::shared_ptr<SPath> PathByStopIDs(TStopID start, TStopID end) const noexcept = 0;

Finds a path in between two stops based on the stop IDs.

**Parameters:**

* start: stop ID of starting stop
* end: stop ID of ending stop

**Return types:**

* shared_ptr to path: if both stop IDs are valid, and a path is found between them
* nullptr: if stops don't exist or no path is found between the stops