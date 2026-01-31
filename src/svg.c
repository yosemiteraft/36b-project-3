/**
 * @file svg.c
 * @brief Implementation of simple SVG drawing interface.
 *
 * Implements the basic functions for creating SVG documents.
 */
#include "svg.h"
#include <stdlib.h>
#include <stdio.h>


/**
 * @brief Opaque SVG drawing context.
 *
 * Holds the necessary data to implement functions.
 */
struct SVG_CONTEXT{
    svg_write_fn write_fn;
    svg_cleanup_fn cleanup_fn;
    svg_user_context_ptr user;
};


svg_context_ptr svg_create(svg_write_fn write_fn, 
                           svg_cleanup_fn cleanup_fn, 
                           svg_user_context_ptr user, 
                           svg_px_t width, 
                           svg_px_t height){
    if(write_fn == NULL) {
        return NULL;
    }
    if(width <= 0 || height <= 0) {
        return NULL;
    }
    svg_context_ptr context = (svg_context_ptr) malloc(sizeof(svg_context_t));
    context->write_fn = write_fn;
    context->cleanup_fn = cleanup_fn;
    context->user = user;
    // https://man7.org/linux/man-pages/man3/printf.3.html
    // https://www.geeksforgeeks.org/c/snprintf-c-library/
    int len = snprintf(NULL, 0, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">\n", width, height);
    char* str = (char*) malloc((len+1)*sizeof(char));
    snprintf(str, len+1, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">\n", width, height);
    write_fn(user, str);
    free(str);
    return context;
}

svg_return_t svg_destroy(svg_context_ptr context){
    if(context) {
        svg_return_t result = context->write_fn(context->user, "</svg>\n");
        free(context);
        return result;
    }
    return SVG_OK;
}

svg_return_t svg_circle(svg_context_ptr context,
                        const svg_point_t *center,
                        svg_real_t radius,
                        const char *style){
    if(context) {
        if(center == NULL) {
            return SVG_ERR_NULL;
        }
        if(radius <= 0 || center->x < 0 || center->y < 0) {
            return SVG_ERR_INVALID_ARG;
        }
        int len = snprintf(NULL, 0, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" style=\"%s\"/>\n", center->x, center->y, radius, style);
        char* str = (char*) malloc((len+1)*sizeof(char));
        snprintf(str, len+1, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" style=\"%s\"/>\n", center->x, center->y, radius, style);
        svg_return_t result = context->write_fn(context->user, str);
        free(str);
        return result;
    }
    return SVG_ERR_NULL;
}


svg_return_t svg_rect(svg_context_ptr context,
                      const svg_point_t *top_left,
                      const svg_size_t *size,
                      const char* style){
    if(context) {
        if(top_left == NULL || size == NULL) {
            return SVG_ERR_NULL;
        }
        if(size->height <= 0 || size->width <= 0) {
            return SVG_ERR_INVALID_ARG;
        }
        // https://www.w3schools.com/graphics/svg_rect.asp
        int len = snprintf(NULL, 0, "<rect width=\"%lf\" height=\"%lf\" x=\"%lf\" y=\"%lf\" style=\"%s\"/>\n", size->width, size->height, top_left->x, top_left->y, style);
        char* str = (char*) malloc((len+1)*sizeof(char));
        snprintf(str, len+1, "<rect width=\"%lf\" height=\"%lf\" x=\"%lf\" y=\"%lf\" style=\"%s\"/>\n", size->width, size->height, top_left->x, top_left->y, style);
        svg_return_t result = context->write_fn(context->user, str);
        free(str);
        return result;
    }
    return SVG_ERR_NULL;
}

svg_return_t svg_line(svg_context_ptr context,
                      const svg_point_t *start,
                      const svg_point_t *end,
                      const char* style){
    if(context) {
        if(start == NULL || end == NULL) {
            return SVG_ERR_NULL;
        }
        int len = snprintf(NULL, 0, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"%s\"/>\n", start->x, start->y, end->x, end->y, style);
        char* str = (char*) malloc((len+1)*sizeof(char));
        snprintf(str, len+1, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"%s\"/>\n", start->x, start->y, end->x, end->y, style);
        svg_return_t result = context->write_fn(context->user, str); 
        free(str);
        return result;
    }
    return SVG_ERR_NULL;
}

svg_return_t svg_group_begin(svg_context_ptr context, 
                             const char* attrs){
    if(context) {
        // https://developer.mozilla.org/en-US/docs/Web/SVG/Reference/Element/g
        int len = snprintf(NULL, 0, "<g %s>\n", attrs);
        char* str = (char*) malloc((len+1)*sizeof(char));
        snprintf(str, len+1, "<g %s>\n", attrs);
        svg_return_t result = context->write_fn(context->user, str);
        free(str);
        return result;
    }
    return SVG_ERR_NULL;
}

svg_return_t svg_group_end(svg_context_ptr context){
    if(context) {
        svg_return_t result = context->write_fn(context->user, "</g>\n");
        return result;
    }
    return SVG_ERR_NULL;
}
