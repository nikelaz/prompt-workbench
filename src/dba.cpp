#include "dba.h"
#include <iostream>

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
        FOREIGN KEY (result_run_id) REFERENCES result_runs(id) ON DELETE CASCADE
        );
    )";
}


bool dba::init(const std::string& db_path)
{
    try
    {
        db = std::make_unique<sqlite::database>(db_path);
        (*db) << "PRAGMA foreign_keys = ON;";
        dba::create_tables();
    }
    catch (const sqlite::sqlite_exception& e)
    {
        std::cerr << "DB init error: " << e.what() << "\n";
        return false;
    }

    return true;
}

int dba::create_test_suite(
    const std::string& title, 
    const std::string& description,
    const std::string& system_prompt,
    const std::string& model
)
{
    try
    {
        (*db) << R"(
            INSERT INTO test_suites
            (title, description, system_prompt, model)
            values (?, ?, ?, ?)
        )"
            << title
            << description
            << system_prompt
            << model;

        return db->last_insert_rowid();
    } 
    catch (const sqlite::sqlite_exception& e)
    {
        std::cerr << "SQLite error: " << e.what() 
                  << " (code " << e.get_code() << ")" << std::endl;
        return -1;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return -1;
    }
}

std::unique_ptr<TestSuite> dba::get_test_suite(int id)
{
    std::string title;
    std::string description;
    std::string system_prompt;
    std::string model;

    try
    {
        (*db) << R"( 
            SELECT
            title, description, system_prompt, model
            FROM test_suites
            WHERE id == ?;
        )"
              << id
              >> std::tie(title, description, system_prompt, model);

        TestSuite test_suite = {
            id,
            title,
            description,
            system_prompt,
            model
        };

        return std::make_unique<TestSuite>(test_suite);
    }
    catch (const sqlite::exceptions::no_rows& e) {
        return nullptr;
    }
}

std::vector<TestSuite> dba::get_all_test_suites()
{
    std::vector<TestSuite> test_suites;
    try
    {
        (*db) << R"( 
            SELECT
            id, title, description, system_prompt, model
            FROM test_suites;
        )"
            >> [&](
                int id,
                std::string title,
                std::string description,
                std::string system_prompt,
                std::string model
            )
            {
               test_suites.push_back(TestSuite {
                    id,
                    title,
                    description,
                    system_prompt,
                    model
                }); 
            };
        return test_suites;
    }
    catch (const sqlite::exceptions::no_rows& e) {
        return test_suites;
    }
}
