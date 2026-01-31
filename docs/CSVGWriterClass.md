# CSVGWriter

CSVGWriter is a tool that writes SVG (scalable vector graphic) files.

## Constructor

CSVGWriter(std::shared_ptr< CDataSink > sink, TSVGPixel width, TSVGPixel height)

Creates an SVG writer with the canvas width and height specified. 

**Parameters:**

* sink: shared pointer to an instance of CDataSink. SVG data will be written into this memory buffer.
* width: specified width of the SVG canvas
* height: specified height of the SVG canvas

**Example usage:**

    std::shared_ptr <CStringDataSink> sink = std::make_shared<CStringDataSink>();
    CSVGWriter writer(sink, 100, 100);

## Member Functions

### Circle

bool Circle(const SSVGPoint &center, TSVGReal radius, const TAttributes &style)

Draws a circle onto the SVG canvas with the specified center point, radius, and style.

**Parameters:**

* center: center of circle to be drawn
* radius: radius of circle to be drawn (> 0)
* style: attributes for circle style

**Return types:**

* true: circle is drawn successfully
* false: error occurred while drawing circle

**Example usage:**

SSVGPoint center = {50, 50};
CSVGWriter::TAttributes style = {{"fill","none"}, 
                                {"stroke", "green"}, 
                                {"stroke-width", "3"}};
writer.Circle(center, 45, style);

### Rectangle

bool Rectange(const SSVGPoint &topleft, const SSVGSize &size, const TAttributes &style)

Draws a rectangle onto the SVG canvas with the specified top left point, size, and style.

**Parameters:**

* topleft: top left corner point of the rectangle
* size: width and height of rectangle (both > 0)
* style: attributes for rectangle style

**Return types:**

* true: rectangle is successfully drawn
* false: error occurred while drawing rectangle

**Example usage:**

SSVGPoint topleft = {5, 5};
SSVGSize size = {20, 50};
CSVGWriter::TAttributes style = {{"fill","none"}, 
                                {"stroke", "green"}, 
                                {"stroke-width", "3"}};
writer.Rectangle(topleft, size, style);

### Line

bool Line(const SSVGPoint &start, const SSVGPoint &end, const TAttributes &style)

Draws a line onto the SVG canvas with the specified start point, end point, and style.

**Parameters:**

* start: start point of the line
* end: end point of the line
* style: attributes for line style

**Return types:**

* true: line is successfully drawn
* false: error occurred while drawing line

**Example usage:**

SSVGPoint start = {0,0};
SSVGPoint end = {50, 50};
CSVGWriter::TAttributes style = {{"fill","none"}, 
                                {"stroke", "green"}, 
                                {"stroke-width", "3"}};
writer.Line(start, end, style);

### SimplePath

bool SimplePath(const std::vector<SSVGPoint> points, const TAttributes &style)

Draws a path of straight lines, connecting a number points in a specified style.

**Parameters:**
* points: vector array of specified points to connect (>= 2 points)
* style: attributes for simple path style

**Return types:**

* true: simple path is successfully drawn
* false: error occurred while drawing simple path

**Example usage:**

const std::vector<SSVGPoint> points =   {{0, 10},
                                        {10, 25},
                                        {40, 60}};
CSVGWriter::TAttributes style = {{"fill","none"}, 
                                {"stroke", "green"}, 
                                {"stroke-width", "3"}};
writer.SimplePath(points, style);

### GroupBegin

bool GroupBegin(const TAttributes &attrs)

Begins an SVG group with an opening <g> tag with specified attributes. Any elements following will be part of the group until GroupEnd() is called.

**Parameters:**

* attrs: attributes to be applied to group

**Return types:**

* true: group is successfully begun
* false: error occurred while beginning group

**Example usage:**
CSVGWriter::TAttributes attrs = {{"fill","none"}, 
                                {"stroke", "green"}, 
                                {"stroke-width", "3"}};
writer.GroupBegin(attrs);

### GroupEnd

bool GroupEnd()

Ends current SVG group with a closing </g> tag.

**Return types:**
* true: group is successfully ended
* false: error occurred while ending group

**Example usage:**
CSVGWriter::TAttributes attrs = {{"fill","none"}, 
                                {"stroke", "green"}, 
                                {"stroke-width", "3"}};
SSVGPoint center = {50, 50};
writer.GroupBegin(attrs);
writer.Circle(center, 45, {});
writer.GroupEnd();
