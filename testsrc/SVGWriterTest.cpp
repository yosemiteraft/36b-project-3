#include <gtest/gtest.h>
#include "SVGWriter.h"
#include "StringDataSink.h"

TEST(SVGWriterTest, CreateDestroyTest){
    /* Tests for:
    * - Correct creation of SVG writer
    * - Generated SVG has correct output structure, including header and element */

    std::shared_ptr <CStringDataSink> Sink = std::make_shared<CStringDataSink>();
    {
        CSVGWriter Writer(Sink, 100, 100);
    }
    EXPECT_EQ(Sink->String(), "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<svg width=\"100\" height=\"100\" xmlns=\"http://www.w3.org/2000/svg\">\n</svg>\n");
}

TEST(SVGWriterTest, CircleTest){
    /* Tests for:
    * - Correct drawing of circle
    * - Valid parameters
    * - Correct style attributes
    * - Can have empty style */

    std::shared_ptr <CStringDataSink> Sink = std::make_shared<CStringDataSink>();
    CSVGWriter Writer(Sink, 100, 100);
    SSVGPoint center = {50, 50};
    TAttributes style = {{"fill", "test"}, {"stroke", "test"}, {"stroke-width", "test"}};
    TAttributes empty_style = {};

    // https://google.github.io/googletest/reference/assertions.html
    EXPECT_TRUE(Writer.Circle(center, 45, style));
    EXPECT_TRUE(Writer.Circle(center, 45, empty_style));
    EXPECT_FALSE(Writer.Circle(center, 0, style));
    EXPECT_FALSE(Writer.Circle(center, -10, style));

    // https://cplusplus.com/reference/string/String/Find/
    std::string output = Sink->String();
    EXPECT_NE(output.find("<circle cx=\"50.000000\" cy=\"50.000000\" r=\"45.000000\" style=\"fill:test; stroke:test; stroke-width:test\"/>\n"), std::string::npos);
}

TEST(SVGWriterTest, RectangleTest){
    /* Tests for:
    * - Correct drawing of rectangle
    * - Valid parameters
    * - Correct style attributes 
    * - Can have empty style */

    std::shared_ptr <CStringDataSink> Sink = std::make_shared<CStringDataSink>();
    CSVGWriter Writer(Sink, 100, 100);
    SSVGPoint topleft = {50, 50};
    SSVGSize size = {50, 50};
    SSVGSize zero_width = {0, 50};
    SSVGSize zero_height = {50, 0};
    SSVGSize neg_width = {-50, 50};
    SSVGSize neg_height = {50, -50};
    TAttributes style = {{"fill", "test"}, {"stroke", "test"}, {"stroke-width", "test"}};
    TAttributes empty_style = {};

    EXPECT_TRUE(Writer.Rectange(topleft, size, style));
    EXPECT_TRUE(Writer.Rectange(topleft, size, empty_style));
    EXPECT_FALSE(Writer.Rectange(topleft, zero_width, style));
    EXPECT_FALSE(Writer.Rectange(topleft, zero_height, style));
    EXPECT_FALSE(Writer.Rectange(topleft, neg_width, style));
    EXPECT_FALSE(Writer.Rectange(topleft, neg_height, style));

    std::string output = Sink->String();
    EXPECT_NE(output.find("<rect width=\"50.000000\" height=\"50.000000\" x=\"50.000000\" y=\"50.000000\" style=\"fill:test; stroke:test; stroke-width:test\"/>\n"), std::string::npos);
}

TEST(SVGWriterTest, LineTest){
    /* Tests for:
    * - Correct drawing of line
    * - Valid parameters
    * - Correct style attributes 
    * - Correct start and end points 
    * - Can have empty style */

    std::shared_ptr <CStringDataSink> Sink = std::make_shared<CStringDataSink>();
    CSVGWriter Writer(Sink, 100, 100);
    SSVGPoint start = {0, 0};
    SSVGPoint end = {50, 50};
    TAttributes style = {{"fill", "test"}, {"stroke", "test"}, {"stroke-width", "test"}};
    TAttributes empty_style = {};

    EXPECT_TRUE(Writer.Line(start, end, style));
    EXPECT_TRUE(Writer.Line(start, end, empty_style));

    std::string output = Sink->String();
    EXPECT_NE(output.find("<line x1=\"0.000000\" y1=\"0.000000\" x2=\"50.000000\" y2=\"50.000000\" style=\"fill:test; stroke:test; stroke-width:test\"/>\n"), std::string::npos);
}

TEST(SVGWriterTest, SimplePathTest){
    /* Tests for:
    * - Correct drawing of simple path (connecting points with multiple straight lines)
    * - Valid parameters
    * - Correct style attributes
    * - Must be at least 2 points provided
    * - Can have empty style */

    std::shared_ptr <CStringDataSink> Sink = std::make_shared<CStringDataSink>();
    CSVGWriter Writer(Sink, 100, 100);
    TAttributes style = {{"fill", "test"}, {"stroke", "test"}, {"stroke-width", "test"}};
    TAttributes empty_style = {};
    const std::vector<SSVGPoint> points =   {{0, 10},
                                            {10, 25},
                                            {40, 60}};
    const std::vector<SSVGPoint> no_points = {};
    const std::vector<SSVGPoint> one_point = {{0, 10}};
    
    EXPECT_TRUE(Writer.SimplePath(points, style));
    EXPECT_TRUE(Writer.SimplePath(points, empty_style));
    EXPECT_FALSE(Writer.SimplePath(no_points, style));
    EXPECT_FALSE(Writer.SimplePath(one_point, style));

    std::string output = Sink->String();
    EXPECT_NE(output.find("<line x1=\"0.000000\" y1=\"10.000000\" x2=\"10.000000\" y2=\"25.000000\" style=\"fill:test; stroke:test; stroke-width:test\"/>\n"), std::string::npos);
    EXPECT_NE(output.find("<line x1=\"10.000000\" y1=\"25.000000\" x2=\"40.000000\" y2=\"60.000000\" style=\"fill:test; stroke:test; stroke-width:test\"/>\n"), std::string::npos);
    
}

TEST(SVGWriterTest, GroupTest){
    /* Tests for:
    * - Groups can be successfully started with attributes provided
    * - Groups can be successfully ended
    * - Resulting output consists of the opening/closing tags */

    // https://developer.mozilla.org/en-US/docs/Web/SVG/Reference/Element/g
    std::shared_ptr <CStringDataSink> Sink = std::make_shared<CStringDataSink>();
    CSVGWriter Writer(Sink, 100, 100);
    TAttributes attrs = {{"fill", "test"}, {"stroke", "test"}};

    EXPECT_TRUE(Writer.GroupBegin(attrs));
    EXPECT_TRUE(Writer.GroupEnd());

    std::string output = Sink->String();
    EXPECT_NE(output.find("<g fill:test; stroke:test>\n"), std::string::npos);
    EXPECT_NE(output.find("</g>\n"), std::string::npos);
}

class CFailingSink : public CDataSink{
    public:
        int DValidCalls = 0;
        virtual ~CFailingSink(){};
        bool Put(const char &ch) noexcept override{
            if(DValidCalls){
                DValidCalls--;
                return true;
            }
            return false;
        }

        bool Write(const std::vector<char> &buf) noexcept override{
            if(DValidCalls){
                DValidCalls--;
                return true;
            }
            return false;
        }
};

TEST(SVGWriterTest, ErrorTests){
    /* Tests for:
    * - Creates failing sink that fails after 1 successful Put() call
    * - Failing after a specified number of single character writes 
    * - Member functions return false when writing to sink is unsuccessful */

    std::shared_ptr<CFailingSink> FailingSink = std::make_shared<CFailingSink>();
    FailingSink->DValidCalls = 1; // 1 successful Put call
    CSVGWriter Writer(FailingSink, 100, 100); // 1st Put call
    SSVGPoint Center = {50, 50};
    TAttributes style = {{"fill", "test"}, {"stroke", "test"}, {"stroke-width", "test"}};

    EXPECT_FALSE(Writer.Circle(Center, 45, style));
    EXPECT_FALSE(Writer.Circle(Center, 50, style));
}
