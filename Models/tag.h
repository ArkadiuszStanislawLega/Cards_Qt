#pragma once
#ifndef TAG
#define TAG
#include <QString>
#include <QObject>
#include <QList>

#include "strings.h"
#include "question.h"
#include "../Database/crud.h"
#include "../Database/manytomany.h"

class Question;

class Tag : public QObject, public Crud<Tag>, public ManyToMany<Question>{
    private:
        Q_OBJECT
        int _id;
        QString _tag;

	public:
        Tag(QObject *parent = nullptr);
        Tag(int, QString, QObject *parent = nullptr);

        int get_id();
        QString get_tag();

        void set_id(int);
        void set_tag(QString);
		bool is_question_already_related(Question *);

        // ManyToMany interface
        bool isRelationCreated(Question *);
        bool isRemovedRelation(Question *);
        QList<Question *> getAllRelated();
        bool isAllRelationRemoved();

        // Db_crud interface
        bool isCreate();
        Tag *isRead();
        bool isUpdate();
        bool isRemoved();
        int findId();
        static QList<Tag *> getAll();
};
#endif
