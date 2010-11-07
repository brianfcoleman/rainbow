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
#include "IntegerSize.hpp"

namespace VideoCapture {

IntegerSize::IntegerSize()
    : m_width(0),
      m_height(0) {

}

IntegerSize::IntegerSize(
    const PRInt32 width,
    const PRInt32 height)
    : m_width(width),
      m_height(height) {

}

PRInt32 IntegerSize::width() const {
  return m_width;
}

PRInt32 IntegerSize::height() const {
  return m_height;
}

bool IntegerSize::isValid() const {
  if (m_width <= 0) {
    return false;;
  }
  if (m_height <= 0) {
    return false;
  }
  return true;
}

bool operator==(const IntegerSize& lhs, const IntegerSize& rhs) {
  if (!lhs) {
    return false;
  }
  if (!rhs) {
    return false;
  }
  if (lhs.width() != rhs.width()) {
    return false;
  }
  if (lhs.height() != rhs.height()) {
    return false;
  }
  return true;
}

bool operator!=(const IntegerSize& lhs, const IntegerSize& rhs) {
  if (!lhs) {
    return true;
  }
  if (!rhs) {
    return true;
  }
  if (lhs.width() == rhs.width()) {
    return false;
  }
  if (lhs.height() == rhs.height()) {
    return false;
  }
  return true;
}

} // VideoCapture
