/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <string.h>

#include "json-util.h"

int json2text(json_t const* json,char *text){
    long text_len = 0;
    
    jsonType_t const type = json_getType( json );
    if ( type != JSON_OBJ && type != JSON_ARRAY ) {
        sprintf(text+text_len,"INVALID JSON OBJECT");
        return -1;
    }

    sprintf(text+text_len,"%s", type == JSON_OBJ? "{": "[" );
    text_len += 1;
    
    json_t const* child;
    for( child = json_getChild( json ); child != 0; child = json_getSibling( child ) ) {

        jsonType_t propertyType = json_getType( child );
        char const* name = json_getName( child );
        if ( name ) {
            sprintf(text+text_len,"\"%s\":", name );
            text_len += strlen(name)+3;
        }

        if ( propertyType == JSON_OBJ || propertyType == JSON_ARRAY )
            text_len += json2text(child,text+text_len);
        else {
            char const* value = json_getValue( child );
            if ( value ) {
                bool const _text = JSON_TEXT == json_getType( child );
                char const* fmt = _text? "\"%s\"":" %s";
                sprintf( text+text_len,fmt, value );
                text_len += text? strlen(value)+2 : strlen(value);
                bool const last = !json_getSibling( child );
                if ( !last ) {
                    sprintf(text+text_len,",");
                    text_len += 1;
                }
            }
        }
    }
    sprintf(text+text_len, "%s", type == JSON_OBJ? "}": "]" );
    text_len += 1;
    return text_len;
}