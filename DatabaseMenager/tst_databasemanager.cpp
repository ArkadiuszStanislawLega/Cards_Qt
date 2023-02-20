#include <QtTest>
#include "../Models/question.h"
#include "../Models/tag.h"
#include "../Database/dbquestion.h"
#include "../Database/dbtag.h"
#include "../Database/dbmanager.h"
#include "../Database/dbrelationquestiontag.h"
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

    void isCreateTag();
    void isFindIdTag();
    void readTag();
    void isUpdateTag();
    void getAllTags();

    void isRelationCreated();
    void readAllRelatedQuestions();
    void readAllRelatedTags();

    void isRelationRemoved();
    void isAllRealtionWithQuestionRemoved();

    void isRelationWithTagRemoved();

    void isRemovedQuestion();
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
    QVERIFY(DbQuestion::getAllQuestions().size() > 0);
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
    QString test_value_2 = "TEST2";
    Tag *t = new Tag(this->tag_id, test_value);
    t->set_tag(test_value_2);

    QVERIFY(DbTag::isUpdate(t));

    t = DbTag::read(this->tag_id);
     QCOMPARE(test_value_2, t->get_tag());
}

void DatabaseManager::getAllTags(){
    QVERIFY(DbTag::getAllTags().size() > 0);
}

void DatabaseManager::isRemovedTag(){
    QVERIFY(DbTag::isRemoved(this->tag_id));
}

void DatabaseManager::isRelationCreated(){
    Tag *t = new Tag(this->tag_id, test_value);
    Question *q = new Question(this->question_id, test_value, test_value, {});
    QVERIFY(DbRelationQuestionTag::isRelationCreated(q, t));
}

void DatabaseManager::readAllRelatedQuestions(){
}

void DatabaseManager::isRelationWithTagRemoved(){
    Tag *t = new Tag(this->tag_id, test_value);
    QString value1 = "q1", value2 = "q2";

    Question *q = new Question();
    q->set_answer(value1);
    q->set_value(value1);

    Question *q1 = new Question();
    q1->set_answer(value2);
    q1->set_value(value1);

    DbQuestion::isCreate(q);
    DbQuestion::isCreate(q1);

    q->set_id(DbQuestion::findId(value1, value1));
    q1->set_id(DbQuestion::findId(value2, value2));

    DbRelationQuestionTag::isRelationCreated(q, t);
    DbRelationQuestionTag::isRelationCreated(q1, t);

    QVERIFY(DbRelationQuestionTag::isAllRelationWithQuestionRemoved(q));
    QVERIFY(DbRelationQuestionTag::readRelatedTags(q).size() == 0);
    QVERIFY(DbRelationQuestionTag::readRelatedTags(q1).size() == 0);

    DbQuestion::isRemoved(q->get_id());
    DbQuestion::isRemoved(q1->get_id());
}

void DatabaseManager::readAllRelatedTags(){
    Question *q = new Question(this->question_id, test_value, test_value, {});
    QVERIFY(DbRelationQuestionTag::readRelatedTags(q).size() > 0);
}

void DatabaseManager::isRelationRemoved(){
    Tag *t = new Tag(this->tag_id, test_value);
    Question *q = new Question(this->question_id, test_value, test_value, {});
    QVERIFY(DbRelationQuestionTag::isRelationRemoved(q, t));
}

void DatabaseManager::isAllRealtionWithQuestionRemoved(){
    Question *q;
    Tag *t1, *t2;
    QString tag1, tag2;
    q = new Question(this->question_id, test_value, test_value, {});
    t1 = new Tag();
    t2 = new Tag();

    tag1 = "tag1";
    tag2 = "tag2";

    t1->set_tag(tag1);
    t2->set_tag(tag2);

    DbRelationQuestionTag::isRelationCreated(q, t1);
    DbRelationQuestionTag::isRelationCreated(q, t2);
    QVERIFY(DbRelationQuestionTag::isAllRelationWithQuestionRemoved(q));
    QVERIFY(DbRelationQuestionTag::readRelatedTags(q).size() == 0);
}

QTEST_APPLESS_MAIN(DatabaseManager)

#include "tst_databasemanager.moc"
