#ifndef LEARNVIEW_H
#define LEARNVIEW_H

#include <QWidget>
#include <QStringListModel>
#include <vector>
#include "../Models/question.h"
#include "../Models/tag.h"
#include "../Constants/strings.h"
#include "ui_learnview.h"

namespace Ui{
        class LearnView;
}

class LearnView : public QWidget
{

private:
    Q_OBJECT
    Ui::LearnView *ui;
    QStringListModel *_tags_model;
    QList<Tag *>_tags_list;
    QList<Question *> _randomised_questions;
    Question *_current;
    int _selected_index, _correct_answer, _uncorrect_answer, _max_questions_number;

    QList<unsigned int> _max_question_number_in_tag;

    void initialTagListView();
    void select_questions();
    void set_value();
    void set_answer();
    void action_after_set_points();
    void make_randomised_questions_list_new();

public:
    explicit LearnView(QWidget *parent = nullptr);
    ~LearnView();

signals:

public slots:
    void added_question_to_db();
    void removed_question_from_db();
    void added_tag_to_db();
    void update_tag_in_db();
    void removed_tag_from_db();
    void create_relation();
    void remove_relation();

private slots:
    void on_b_start_clicked();
    void on_b_correct_clicked();
    void on_b_uncorrect_clicked();
    void on_b_show_answer_clicked();
    void on_cb_tags_currentIndexChanged(int index);
};

#endif // LEARNVIEW_H
