/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla Labs Rainbow.
 *
 * The Initial Developer of the Original Code is
 * Brian Coleman.
 * Portions created by the Initial Developer are Copyright (C) 2010
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Brian Coleman <brianfcoleman@gmail.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
#include "GUIDUtilities.hpp"

#include "nspr.h"

namespace VideoCapture {

static const PRInt32 kCountElementsData4GUID = 8;

const GUID emptyGUID() {
  GUID guid;
  guid.Data1 = 0;
  guid.Data2 = 0;
  guid.Data3 = 0;
  for (PRInt32 i = 0; i < kCountElementsData4GUID; ++i) {
    guid.Data4[i] = 0;
  }
  return guid;
}

bool isEmptyGUID(const GUID& guid) {
  bool isEmpty = true;
  if (guid.Data1) {
    return !isEmpty;
  }
  if (guid.Data2) {
    return !isEmpty;
  }
  if (guid.Data3) {
    return !isEmpty;
  }
  for (PRInt32 i = 0; i < kCountElementsData4GUID; ++i) {
    if (guid.Data4[i]) {
      return !isEmpty;
    }
  }
  return isEmpty;
}

} // VideoCapture