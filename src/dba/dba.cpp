#include "dba.h"
#include <iostream>

using std::string;

std::unique_ptr<sqlite::database> dba::db;

void dba::create_tables() {
    (*db) << R"(
        CREATE TABLE IF NOT EXISTS test_suites (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        title TEXT NOT NULL,
        description TEXT,
        system_prompt TEXT NOT NULL,
        model TEXT NOT NULL
        );
    )";

    (*db) << R"(
        CREATE TABLE IF NOT EXISTS user_prompts (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        prompt TEXT NOT NULL,
        test_suite_id INTEGER NOT NULL,
        FOREIGN KEY (test_suite_id) REFERENCES test_suites(id) ON DELETE CASCADE
        );
    )";

    (*db) << R"(
        CREATE TABLE IF NOT EXISTS result_runs (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        date TEXT NOT NULL,
        test_suite_id INTEGER NOT NULL,
        FOREIGN KEY (test_suite_id) REFERENCES test_suites(id) ON DELETE CASCADE
        );
    )";

    (*db) << R"(
        CREATE TABLE IF NOT EXISTS answers (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        answer TEXT NOT NULL,
        result_run_id INTEGER NOT NULL,
        user_prompt_id INTEGER NOT NULL,
        FOREIGN KEY (result_run_id) REFERENCES result_runs(id) ON DELETE CASCADE
        FOREIGN KEY (user_prompt_id) REFERENCES user_prompts(id) ON DELETE CASCADE
        );
    )";
}

bool dba::init(const string& db_path)
{
    try
    {
        db = std::make_unique<sqlite::database>(db_path);
        (*db) << "PRAGMA foreign_keys = ON;";
        dba::create_tables();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return false;
    }

    return true;
}
