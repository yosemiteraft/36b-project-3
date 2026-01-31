#include "SVGWriter.h"
#include "svg.h"
#include <string>
#include <vector>
#include <iostream>

using std::cout;
using std::endl;

struct CSVGWriter::SImplementation{
    svg_context_ptr DContext;
    // https://stackoverflow.com/questions/6876751/differences-between-unique-ptr-and-shared-ptr
    std::shared_ptr<CDataSink> DSink;

    static svg_return_t WriteCallback(svg_user_context_ptr user, const char* text) { // dont need instance of object to call it, callback must be passed to c
        if(!user || !text) {
            return SVG_ERR_NULL;
        }
        SImplementation *Implementation = (SImplementation*) user;
        while(*text) { // while not null,
            // Dereference pointer and evaluates to character being pointed to, then post increments pointer to move to next character
            if(!Implementation->DSink->Put(*text)) {
                return SVG_ERR_IO;
            }
            text++;
        }
        return SVG_OK;
    }

    SImplementation(std::shared_ptr< CDataSink > sink, TSVGPixel width, TSVGPixel height) {
        DSink = sink;
        DContext = svg_create(WriteCallback, nullptr, this, width, height);
    }

    ~SImplementation() {
        // https://www.geeksforgeeks.org/cpp/destructors-c/
        svg_destroy(DContext);
    }

    std::string CreateStyleStr(const TAttributes &style) {
        std::string str;
        for(const TAttribute &Attr : style) {
            // https://stackoverflow.com/questions/41425569/checking-for-empty-string-in-c-alternatives
            // https://learncplusplus.org/how-do-i-add-characters-to-strings-in-c-the-right-way/
            // https://stackoverflow.com/questions/347949/how-to-convert-a-stdstring-to-const-char-or-char
            if(str.length() != 0) { 
                str += "; "; // don't want to add semicolon if string is empty
            }
            std::string key = std::get<0>(Attr);
            std::string value = std::get<1>(Attr);
            str += key;
            str += ":";
            str += value;
        }
        return str;
    }

    bool Circle(const SSVGPoint &center, TSVGReal radius, const TAttributes &style){
        // https://www.w3schools.com/cpp/cpp_vectors.asp
        svg_point_t Center{center.DX, center.DY};
        std::string Style = CreateStyleStr(style);
        if(radius <= 0) {
            return false;
        }
        return svg_circle(DContext, &Center, radius, Style.c_str()) == SVG_OK;
    }

    bool Rectange(const SSVGPoint &topleft, const SSVGSize &size, const TAttributes &style){
        svg_point_t svg_topleft{topleft.DX, topleft.DY};
        svg_size_t svg_size{size.DWidth, size.DHeight};
        std::string Style = CreateStyleStr(style);
        if(size.DWidth <= 0 || size.DHeight <=0 ) {
            return false;
        }
        return svg_rect(DContext, &svg_topleft, &svg_size, Style.c_str()) == SVG_OK;
    }

    bool Line(const SSVGPoint &start, const SSVGPoint &end, const TAttributes &style){
        svg_point_t svg_start{start.DX, start.DY};
        svg_point_t svg_end{end.DX, end.DY};
        std::string str;
        std::string Style = CreateStyleStr(style);
        return svg_line(DContext, &svg_start, &svg_end, Style.c_str()) == SVG_OK;
    }

    bool SimplePath(const std::vector<SSVGPoint> points, const TAttributes &style){
        if(points.size() < 2) {
            return false;
        }
        std::string Style = CreateStyleStr(style);
        svg_point_t svg_start;
        svg_point_t svg_end;
        // https://www.w3schools.com/cpp/ref_vector_size.asp
        for(size_t i=0; i<points.size()-1; i++) {
            svg_start.x = points[i].DX;
            svg_start.y = points[i].DY;
            svg_end.x = points[i+1].DX;
            svg_end.y = points[i+1].DY;
            svg_line(DContext, &svg_start, &svg_end, Style.c_str());
        }
        return true;
    }

    bool GroupBegin(const TAttributes &attrs){
        std::string Attrs = CreateStyleStr(attrs);
        return svg_group_begin(DContext, Attrs.c_str()) == SVG_OK;
    }

    bool GroupEnd(){
        return svg_group_end(DContext) == SVG_OK;
    }

};

CSVGWriter::CSVGWriter(std::shared_ptr< CDataSink > sink, TSVGPixel width, TSVGPixel height){
    // https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
    DImplementation = std::make_unique<SImplementation>(sink, width, height);
    
}

CSVGWriter::~CSVGWriter(){

}

bool CSVGWriter::Circle(const SSVGPoint &center, TSVGReal radius, const TAttributes &style){
    return DImplementation->Circle(center, radius, style);
}

bool CSVGWriter::Rectange(const SSVGPoint &topleft, const SSVGSize &size, const TAttributes &style){
    return DImplementation->Rectange(topleft, size, style);
}

bool CSVGWriter::Line(const SSVGPoint &start, const SSVGPoint &end, const TAttributes &style){
    return DImplementation->Line(start, end, style);
}

bool CSVGWriter::SimplePath(const std::vector<SSVGPoint> points, const TAttributes &style){
    return DImplementation->SimplePath(points, style);
}

bool CSVGWriter::GroupBegin(const TAttributes &attrs){
    return DImplementation->GroupBegin(attrs);
}

bool CSVGWriter::GroupEnd(){
    return DImplementation->GroupEnd();
}