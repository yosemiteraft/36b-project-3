# CXMLBusSystem

CXMLBusSystem is a class that parses bus system data from XML files. This includes bus stops, routes, and paths.

## Constructor
CXMLBusSystem(std::shared_ptr< CXMLReader > systemsource, std::shared_ptr< CXMLReader > pathsource);

Creates a XMLBusSystem class using two different XML readers to parse bus system data.

**Parameters:**

* systemsource: XML reader with bus system data, including bus stops and routes
* pathsource: XML reader with path data, including nodes along paths

**Example usage:**

auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                            "<stops>\n"
                                                            "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                            "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                            "   <stop id=\"3\" node=\"301\" description=\"third\"/>\n"
                                                            "   <stop id=\"4\" node=\"291\" description=\"fourth\"/>\n"
                                                            "</stops>\n"
                                                            "</bussystem>");
auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                            "   <path source=\"321\" destination=\"311\">\n"
                                                            "      <node id=\"321\"/>\n"
                                                            "      <node id=\"315\"/>\n"
                                                            "      <node id=\"311\"/>\n"
                                                            "   </path>\n"
                                                            "   <path source=\"311\" destination=\"301\">\n"
                                                            "      <node id=\"311\"/>\n"
                                                            "      <node id=\"317\"/>\n"
                                                            "      <node id=\"320\"/>\n"
                                                            "      <node id=\"301\"/>\n"
                                                            "   </path>\n"
                                                            "   <path source=\"301\" destination=\"291\">\n"
                                                            "   </path>\n"
                                                            "</paths>");
auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

## Member Functions

### StopCount

std::size_t StopCount() const noexcept override;

Returns total number of bus stops in system.

**Return types:**

* std::size_t: number of stops in the system

**Example usage:**

auto BusSystem = CXMLBusSystem(systemsource, pathsource);
std::size_t NumStops = BusSystem.StopCount();

### RouteCount

std::size_t RouteCount() const noexcept override;

Returns total number of bus routes in system.

**Return types:**

* std::size_t: number of routes in system

**Example usage:**

auto BusSystem = CXMLBusSystem(systemsource, pathsource);
std::size_t NumRoutes = BusSystem.RouteCount();

### StopByIndex

std::shared_ptr<SStop> StopByIndex(std::size_t index) const noexcept override;

Accesses a bus stop by its index.

**Parameters:**

* index: index of stop to access

**Return types:**

* shared_ptr to stop: if valid index provided
* nullptr: invalid index provided

**Example usage:**

auto BusSystem = CXMLBusSystem(systemsource, pathsource);
auto StopObj = BusSystem.StopByIndex(0);

### StopByID

std::shared_ptr<SStop> StopByID(TStopID id) const noexcept override;

Accesses a bus stop by its stop ID.

**Parameters:**

* id: a unique ID associated with a particular stop

**Return types:**

* shared_ptr to stop: if valid ID provided
* nullptr: invalid ID provided

**Example usage:**

auto BusSystem = CXMLBusSystem(systemsource, pathsource);
auto Stop = BusSystem.StopByID(1);

### RouteByIndex

std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const noexcept override;

Accesses a bus route by its index.

**Parameters:**

* index: index of bus route to access

**Return types:**

* shared_ptr to route: if valid index provided
* nullptr: invalid index provided

**Example usage:**

auto BusSystem = CXMLBusSystem(systemsource, pathsource);
auto Route = BusSystem.RouteByIndex(0);

### RouteByName

std::shared_ptr<SRoute> RouteByName(const std::string &name) const noexcept override;

Accesses a bus route by its name.

**Parameters:**

* name: name of bus route to access

**Return types:**

* shared_ptr to route: if valid, existing name provided
* nullptr: if invalid name provided

**Example usage:**

auto BusSystem = CXMLBusSystem(systemsource, pathsource);
auto Route = BusSystem.RouteByName("A");

### PathByStopIDs

std::shared_ptr<SPath> PathByStopIDs(TStopID start, TStopID end) const noexcept override;

Finds a path in between two stops based on the stop IDs.

**Parameters:**

* start: stop ID of starting stop
* end: stop ID of ending stop

**Return types:**

* shared_ptr to path: if both stop IDs are valid, and a path is found between them
* nullptr: if stops don't exist or no path is found between the stops

**Example usage:**

auto BusSystem = CXMLBusSystem(systemsource, pathsource);
auto Path = BusSystem.PathByStopIDs(1,2);