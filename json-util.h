/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   json-util.h
 * Author: prodev-ahz
 *
 * Created on August 30, 2018, 4:59 PM
 */

#ifndef JSON_UTIL_H
#define JSON_UTIL_H

#include "tiny-json.h"

#ifdef __cplusplus
extern "C" {
#endif

int json2text(json_t const* json,char *text);

#ifdef __cplusplus
}
#endif

#endif /* JSON_UTIL_H */

