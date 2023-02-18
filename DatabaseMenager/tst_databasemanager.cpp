#include <QtTest>
#include "../Models/question.h"
#include "../Models/tag.h"
#include "../Database/dbquestion.h"
#include "../Database/dbtag.h"
#include "../Database/dbmanager.h"
#include "../Constants/strings.h"

class DatabaseManager : public QObject
{
    Q_OBJECT
    const QString test_value = "TEST";
    int question_id {0};
    int tag_id {0};

public:
    DatabaseManager();
    ~DatabaseManager();

private slots:
    void isCreateQuestion();
    void findIdQuestion();
    void readQuestion();
    void isUpdateQuestion();
    void getAllQuestions();
    void isRemovedQuestion();

    void isCreateTag();
    void isFindIdTag();
    void readTag();
    void isUpdateTag();
    void getAllTags();
    void isRemovedTag();
};

DatabaseManager::DatabaseManager()
{
    DbManager(DATABASE_NAME);
}

DatabaseManager::~DatabaseManager()
{

}

void DatabaseManager::isCreateQuestion()
{
    Question *q = new Question();
    q->set_answer(test_value);
    q->set_value(test_value);
    QVERIFY(DbQuestion::isCreate(q));
}

void DatabaseManager::findIdQuestion(){
    this->question_id = DbQuestion::findId(test_value, test_value);
    QVERIFY(this->question_id > 0);
}

void DatabaseManager::readQuestion(){
    Question *q = DbQuestion::read(this->question_id);
    QCOMPARE(this->question_id, q->get_id());
    QCOMPARE(test_value, q->get_answer());
    QCOMPARE(test_value, q->get_value());
}

void DatabaseManager::isUpdateQuestion(){
    QString test_value_2 = "TEST2";
    Question *q = new Question(this->question_id, test_value, test_value, {});
    q->set_answer(test_value_2);
    q->set_value(test_value_2);

    QVERIFY(DbQuestion::isUpdate(q));

    q = DbQuestion::read(this->question_id);
    QCOMPARE(test_value_2, q->get_answer());
    QCOMPARE(test_value_2, q->get_value());
}

void DatabaseManager::getAllQuestions(){
    QCOMPARE(1, DbQuestion::getAllQuestions().size());
}

void DatabaseManager::isRemovedQuestion(){
    QVERIFY(DbQuestion::isRemoved(this->question_id));
}

void DatabaseManager::isCreateTag(){
    Tag *t = new Tag();
    t->set_tag(test_value);
    QVERIFY(DbTag::isCreate(t));
}

void DatabaseManager::isFindIdTag(){
    this->tag_id = DbTag::findId(test_value);
    QVERIFY(this->tag_id > 0);
}

void DatabaseManager::readTag(){
    Tag *t = DbTag::read(this->tag_id);
    QCOMPARE(this->tag_id, t->get_id());
    QCOMPARE(test_value, t->get_tag());
}

void DatabaseManager::isUpdateTag(){

}

void DatabaseManager::getAllTags(){

}

void DatabaseManager::isRemovedTag(){

}

QTEST_APPLESS_MAIN(DatabaseManager)

#include "tst_databasemanager.moc"