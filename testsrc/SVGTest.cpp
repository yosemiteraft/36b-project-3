#include "svg.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

// Helper structure to capture written SVG strings
struct STestOutput{
    std::vector<std::string> DLines;
    bool DDestroyed = false;

    std::string JoinOutput(){
        std::string Result;

        for (const auto& line : DLines){
            Result += line;
        }
        return Result;
    }
};

// Callback to capture SVG output
svg_return_t write_callback(svg_user_context_ptr user, const char* text){
    if(!user || !text){
        return SVG_ERR_NULL;
    }
    STestOutput* OutPtr = static_cast<STestOutput*>(user);
    OutPtr->DLines.push_back(text);
    return SVG_OK;
}

// Cleanup callback (just returns OK for testing)
svg_return_t cleanup_callback(svg_user_context_ptr user){
    if(!user){
        return SVG_ERR_NULL;
    }
    STestOutput* OutPtr = static_cast<STestOutput*>(user);
    if(OutPtr->DDestroyed){
        return SVG_ERR_STATE;
    }
    OutPtr->DDestroyed = true;
    return SVG_OK;
}

// --- TEST FIXTURE ---
class SVGTest : public ::testing::Test{
    protected:
        STestOutput DOutput;
        svg_context_ptr DContext = nullptr;

        void SetUp() override{
            DContext = svg_create(write_callback, cleanup_callback, &DOutput, 100, 100);
            ASSERT_NE(DContext, nullptr);
        }

        void TearDown() override{
            if(DContext){
                svg_destroy(DContext);
                DContext = nullptr;
            }
        }
};

// --- BASIC CREATION TEST ---
TEST_F(SVGTest, CreateAndDestroy){
    // https://google.github.io/googletest/primer.html
    svg_context_ptr context = svg_create(write_callback, cleanup_callback, &DOutput, 100, 100);
    EXPECT_NE(context, nullptr);
    EXPECT_EQ(svg_destroy(context), SVG_OK);
}

// --- INVALID INPUT TESTS ---
TEST_F(SVGTest, NullContextFunctions){
    svg_point_t point = {50, 50};
    svg_size_t size = {50, 50};

    EXPECT_EQ(svg_circle(nullptr, &point, 45, "test"), SVG_ERR_NULL);
    EXPECT_EQ(svg_rect(nullptr, &point, &size, "test"), SVG_ERR_NULL);
    EXPECT_EQ(svg_line(nullptr, &point, &point, "test"), SVG_ERR_NULL);
    EXPECT_EQ(svg_group_begin(nullptr, "test"), SVG_ERR_NULL);
    EXPECT_EQ(svg_group_end(nullptr), SVG_ERR_NULL);
}

// --- DRAWING TESTS ---
TEST_F(SVGTest, Circle){
    svg_point_t center = {50, 50};
    EXPECT_EQ(svg_circle(DContext, &center, 45, "test"), SVG_OK);
    EXPECT_EQ(svg_circle(DContext, &center, 45, NULL), SVG_OK); // Style can be NULL

    // https://cplusplus.com/reference/string/String/Find/
    std::string output = DOutput.JoinOutput();
    EXPECT_NE(output.find("<circle cx=\"50.000000\" cy=\"50.000000\" r=\"45.000000\" style=\"test\"/>\n"), std::string::npos);
    
    EXPECT_EQ(svg_circle(DContext, &center, 0, "test"), SVG_ERR_INVALID_ARG);
    EXPECT_EQ(svg_circle(DContext, &center, -10, "test"), SVG_ERR_INVALID_ARG);
}

TEST_F(SVGTest, Rectangle){
    svg_point_t top_left = {50, 50};
    svg_size_t size = {50, 50};
    svg_size_t zero_height = {0,50};
    svg_size_t zero_width = {50,0};
    svg_size_t neg_height = {0,-50};
    svg_size_t neg_width = {-50,0};
    EXPECT_EQ(svg_rect(DContext, &top_left, &size, "test"), SVG_OK);
    EXPECT_EQ(svg_rect(DContext, &top_left, &size, NULL), SVG_OK); // Style can be NULL

    std::string output = DOutput.JoinOutput();
    EXPECT_NE(output.find("<rect width=\"50.000000\" height=\"50.000000\" x=\"50.000000\" y=\"50.000000\" style=\"test\"/>\n"), std::string::npos);

    EXPECT_EQ(svg_rect(DContext, &top_left, &zero_height, "test"), SVG_ERR_INVALID_ARG);
    EXPECT_EQ(svg_rect(DContext, &top_left, &zero_width, "test"), SVG_ERR_INVALID_ARG);
    EXPECT_EQ(svg_rect(DContext, &top_left, &neg_height, "test"), SVG_ERR_INVALID_ARG);
    EXPECT_EQ(svg_rect(DContext, &top_left, &neg_width, "test"), SVG_ERR_INVALID_ARG);
}

TEST_F(SVGTest, Line){
    svg_point_t start = {0, 0};
    svg_point_t end = {50, 50};
    EXPECT_EQ(svg_line(DContext, &start, &end, "test"), SVG_OK);
    EXPECT_EQ(svg_line(DContext, &start, &start, "test"), SVG_OK);
    EXPECT_EQ(svg_line(DContext, &start, &end, NULL), SVG_OK); // Style can be NULL

    std::string output = DOutput.JoinOutput();
    EXPECT_NE(output.find("<line x1=\"0.000000\" y1=\"0.000000\" x2=\"50.000000\" y2=\"50.000000\" style=\"test\"/>\n"), std::string::npos);
    EXPECT_NE(output.find("<line x1=\"0.000000\" y1=\"0.000000\" x2=\"0.000000\" y2=\"0.000000\" style=\"test\"/>\n"), std::string::npos);
}

// --- GROUPING TEST ---
TEST_F(SVGTest, Grouping){
    EXPECT_EQ(svg_group_begin(DContext, "fill:\"none\" stroke:\"green\""), SVG_OK);
    EXPECT_EQ(svg_group_begin(nullptr, "fill:\"none\" stroke:\"green\""), SVG_ERR_NULL);
    EXPECT_EQ(svg_group_begin(DContext, NULL), SVG_OK); // Attribute string can be NULL

    EXPECT_EQ(svg_group_end(DContext), SVG_OK);
    EXPECT_EQ(svg_group_end(nullptr), SVG_ERR_NULL);

    std::string output = DOutput.JoinOutput();
    EXPECT_NE(output.find("<g fill:\"none\" stroke:\"green\">\n"), std::string::npos);
    EXPECT_NE(output.find("</g>\n"), std::string::npos);
}

// --- EDGE CASES ---
TEST_F(SVGTest, ZeroDimensions){
    EXPECT_EQ(svg_create(write_callback, cleanup_callback, &DOutput, 0, 100), nullptr);
    EXPECT_EQ(svg_create(write_callback, cleanup_callback, &DOutput, 100, 0), nullptr);
}

TEST_F(SVGTest, NullPointPointer){
    svg_point_t point = {50, 50};
    svg_size_t size = {50, 50};

    EXPECT_EQ(svg_circle(DContext, nullptr, 45, "test"), SVG_ERR_NULL);
    EXPECT_EQ(svg_rect(DContext, nullptr, &size, "test"), SVG_ERR_NULL);
    EXPECT_EQ(svg_rect(DContext, &point, nullptr, "test"), SVG_ERR_NULL);
    EXPECT_EQ(svg_line(DContext, nullptr, &point, "test"), SVG_ERR_NULL);
    EXPECT_EQ(svg_line(DContext, &point, nullptr, "test"), SVG_ERR_NULL);
}

TEST_F(SVGTest, CreateEdgeCases){
    EXPECT_EQ(svg_create(nullptr, cleanup_callback, &DOutput, 100, 100), nullptr);
    EXPECT_NE(svg_create(write_callback, nullptr, &DOutput, 100, 100), nullptr);
    EXPECT_EQ(svg_create(write_callback, cleanup_callback, &DOutput, -100, 100), nullptr);
    EXPECT_EQ(svg_create(write_callback, cleanup_callback, &DOutput, 100, -100), nullptr);
}

TEST_F(SVGTest, DestroyEdgeCases){
    svg_context_ptr context = svg_create(write_callback, cleanup_callback, &DOutput, 100, 100);
    EXPECT_EQ(svg_destroy(context), SVG_OK);
    context = nullptr; // Setting context pointer to NULL after freeing memory
    EXPECT_EQ(svg_destroy(context), SVG_OK); // Double destroy
}

// Callback to capture SVG output
svg_return_t write_error_callback(svg_user_context_ptr user, const char* text){
    int *FailureCount = (int *)user;
    if(*FailureCount){
        (*FailureCount)--;
        return SVG_OK;    
    }
    return SVG_ERR_IO;
}

// --- IO Errors ---
TEST_F(SVGTest, IOErrorTest){
    int FailureCount = 1; // write_error_callback can be called 1 time before failure
    svg_point_t point = {50, 50};
    svg_size_t size = {50, 50};
    svg_context_ptr context = svg_create(write_error_callback, cleanup_callback, &FailureCount, 100, 100);

    EXPECT_EQ(svg_circle(context, &point, 45, "test"), SVG_ERR_IO);
    EXPECT_EQ(svg_rect(context, &point, &size, "test"), SVG_ERR_IO);
    EXPECT_EQ(svg_line(context, &point, &point, "test"), SVG_ERR_IO);
}