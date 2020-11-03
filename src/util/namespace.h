/* @file  namespace.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_UTIL_NAMESPACE_H_
#define EDI_UTIL_NAMESPACE_H_

#define EDI_NAMESPACE open_edi::db
#define EDI_DB EDI_NAMESPACE
#define EDI_BEGIN_NAMESPACE namespace open_edi  { namespace db {
#define EDI_END_NAMESPACE }}

#define UTIL_NAMESPACE util
#define UTIL_BEGIN_NAMESPACE namespace UTIL_NAMESPACE {
#define UTIL_END_NAMESPACE }

#endif
