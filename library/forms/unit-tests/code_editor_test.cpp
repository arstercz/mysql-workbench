/*
 * Copyright (c) 2012, 2018, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2.0,
 * as published by the Free Software Foundation.
 *
 * This program is also distributed with certain software (including
 * but not limited to OpenSSL) that is licensed under separate terms, as
 * designated in a particular file or component or in included license
 * documentation.  The authors of MySQL hereby grant you an additional
 * permission to link the program and your derivative works with the
 * separately licensed software that they have included with MySQL.
 * This program is distributed in the hope that it will be useful,  but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License, version 2.0, for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA 
 */

#include "test.h"
#include "mforms/code_editor.h"
#include "stub/stub_mforms.h"
#include "wb_helpers.h"
using namespace mforms;

BEGIN_TEST_DATA_CLASS(mforms_code_editor_test)
protected:
WBTester *_tester; // Initializes the mforms stub with good values (among others).
public:
TEST_DATA_CONSTRUCTOR(mforms_code_editor_test) {
  _tester = new WBTester;
}
END_TEST_DATA_CLASS;

TEST_MODULE(mforms_code_editor_test, "mforms code editor testing");

//----------------------------------------------------------------------------------------------------------------------

TEST_FUNCTION(10) {
  // Copy a current version of the code editor configuration file to the test data folder.
  gchar *contents;
  gsize length;
  GError *error = NULL;
  if (g_file_get_contents("../../res/wbdata/code_editor.xml", &contents, &length, &error)) {
    ensure("Could not write editor configuration to target file",
           g_file_set_contents("data/code_editor.xml", contents, length, &error) == TRUE);
    g_free(contents);
  } else
    fail("Could not copy code editor configuration");

  // Read a pretty standard code editor configuration file (MySQL section). Actual values don't matter (they
  // vary anyway) but there must be some base values in there and the xml must be read successfully.
  CodeEditorConfig config(mforms::LanguageMySQL);
  ensure("No language nodes found", config.get_languages().size() > 0);

  // TODO: change log implementation so that we can easily check for new log entries.

  // Keywords.
  std::map<std::string, std::string> keywords = config.get_keywords();
  ensure("Couldn't read keywords", keywords.size() > 0);
  ensure("Keyword list missing", keywords.find("Keywords") != keywords.end());
  ensure("Keyword list empty", !keywords["Keywords"].empty());
  ensure("Procedure keyword list missing", keywords.find("Procedure keywords") != keywords.end());
  ensure("Procedure keyword list empty", !keywords["Procedure keywords"].empty());
  ensure("User keyword list 1 missing", keywords.find("User Keywords 1") != keywords.end());
  ensure("User keyword list 1 empty", !keywords["User Keywords 1"].empty());

  // Properties.
  std::map<std::string, std::string> properties = config.get_properties();
  ensure("Couldn't read properties", properties.size() > 0);

  // Settings.
  std::map<std::string, std::string> settings = config.get_settings();
  ensure("Couldn't read settings", settings.size() > 0);

  // Styles.
  std::map<int, std::map<std::string, std::string> > styles = config.get_styles();
  ensure("Couldn't read styles", styles.size() > 0);

  // Pick some entries, just to check sub map.
  std::map<std::string, std::string> &values = styles[22]; // SCE_MYSQL_KEYWORD
  ensure("Wrong number of style values found", values.size() > 0);
  ensure("Old style color entry found", values["fore-color"].empty());
  ensure("Missing bold style for MySQL keywords", !values["bold"].empty());

  values = styles[22]; // SCE_MYSQL_PLACEHOLDER
  ensure("Wrong number of style values found", values.size() >= 5);
  ensure("Old style color entry found", values["fore-color"].empty());
  ensure("Missing fore-color-light", !values["fore-color-light"].empty());
  ensure("Missing fore-color-dark", !values["fore-color-dark"].empty());
  ensure("Missing back-color-light", !values["back-color-light"].empty());
  ensure("Missing back-color-dark", !values["back-color-dark"].empty());
  ensure("Missing bold style for MySQL keywords", !values["bold"].empty());
}

//----------------------------------------------------------------------------------------------------------------------

TEST_FUNCTION(20) {
  // Another config check, this time for python.
  CodeEditorConfig config(mforms::LanguagePython);
  ensure("No language nodes found", config.get_languages().size() > 0);

  // Keywords.
  std::map<std::string, std::string> keywords = config.get_keywords();
  ensure("Couldn't read keywords", keywords.size() > 0);
  ensure("Keyword list missing", keywords.find("Keywords") != keywords.end());
  ensure("Keyword list empty", !keywords["Keywords"].empty());

  ensure("Python keyword \"continue\" not in keyword list", strstr(keywords["Keywords"].c_str(), "continue") != NULL);

  // Properties.
  std::map<std::string, std::string> properties = config.get_properties();
  ensure("Couldn't read properties", properties.size() > 0);

  // Settings.
  std::map<std::string, std::string> settings = config.get_settings();
  ensure("Couldn't read settings", settings.size() > 0);

  // Styles.
  std::map<int, std::map<std::string, std::string> > styles = config.get_styles();
  ensure("Couldn't read styles", styles.size() > 0);

  // Pick some entries, just to check sub map.
  std::map<std::string, std::string> values = styles[8]; // Python class name.
  ensure("Invalid style set for Python class names", values.size() > 1);
  ensure("Old style color entry found", values["fore-color"].empty());
  ensure("Missing fore color style for Python class names", !values["fore-color-light"].empty());
  ensure("Missing bold style for Python class names", !values["bold"].empty());
}

//----------------------------------------------------------------------------------------------------------------------

// Due to the tut nature, this must be executed as a last test always,
// we can't have this inside of the d-tor.
TEST_FUNCTION(99) {
  delete _tester;
}

END_TESTS

//----------------------------------------------------------------------------------------------------------------------
