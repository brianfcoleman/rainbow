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
#include "QeditTypeLibraryImpl.hpp"

namespace VideoCapture {

static const nsString s_kTypeLibraryName(NS_LITERAL_STRING("qedit.dll"));

static const nsString s_kIID_ISampleGrabberName(
    NS_LITERAL_STRING("ISampleGrabber"));

static const nsString s_kIID_ISampleGrabberCBName(
    NS_LITERAL_STRING("ISampleGrabberCB"));

static const nsString s_kCLSID_SampleGrabberName(
    NS_LITERAL_STRING("SampleGrabber"));

static const nsString s_kCLSID_NullRendererName(
    NS_LITERAL_STRING("NullRenderer"));

static const nsTArray<nsString> interfaceIdentifierNames();

static const nsTArray<nsString> classIdentifierNames();

static const nsTArray<nsString> interfaceIdentifierNames() {
  nsTArray<nsString> names;
  names.AppendElement(s_kIID_ISampleGrabberName);
  names.AppendElement(s_kIID_ISampleGrabberCBName);
  return names;
}

static const nsTArray<nsString> classIdentifierNames() {
  nsTArray<nsString> names;
  names.AppendElement(s_kCLSID_SampleGrabberName);
  names.AppendElement(s_kCLSID_NullRendererName);
  return names;
}

QeditTypeLibraryImpl::QeditTypeLibraryImpl()
    : TypeLibrary(
        s_kTypeLibraryName,
        interfaceIdentifierNames(),
        classIdentifierNames()) {

}

const IID QeditTypeLibraryImpl::IID_ISampleGrabber() const {
  return TypeLibrary::interfaceIdByName(s_kIID_ISampleGrabberName);
}

const IID QeditTypeLibraryImpl::IID_ISampleGrabberCB() const {
  return TypeLibrary::interfaceIdByName(s_kIID_ISampleGrabberCBName);
}

const CLSID QeditTypeLibraryImpl::CLSID_SampleGrabber() const {
  return TypeLibrary::classIdByName(s_kCLSID_SampleGrabberName);
}

const CLSID QeditTypeLibraryImpl::CLSID_NullRenderer() const {
  return TypeLibrary::classIdByName(s_kCLSID_NullRendererName);
}

} // VideoCapture
