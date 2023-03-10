#include "relationview.h"

RelationView::RelationView(QWidget *parent)
    : QWidget{parent}, ui(new Ui::RelationView){
    this->ui->setupUi(this);
    this->initialQuestionsListView();
    this->initialTagsComboBox();
}

void RelationView::printInfo(const QString &value, bool isError){
    QPalette pal = this->ui->l_info->palette();
    pal.setColor(QPalette::Window, QColor( isError ? Qt::red : Qt::transparent));

    this->ui->l_info->setText(value);
    this->ui->l_info->setPalette(pal);
}

void RelationView::initialQuestionsListView(){
    this->_questions_table_model = new QSqlRelationalTableModel;
    this->_questions_table_model->setTable(TABLE_QUESTIONS);
    this->_questions_table_model->select();

    this->ui->lv_questions->setModel(this->_questions_table_model);
    this->ui->lv_questions->setModelColumn(this->_questions_table_model->record().indexOf(COLUMN_VALUE));
}

void RelationView::initialTagsComboBox(){
    this->_tags_combo_box_model = new QSqlRelationalTableModel;
    this->_tags_combo_box_model->setTable(TABLE_TAGS);
    this->_tags_combo_box_model->select();

    this->ui->cb_tags->setModel(this->_tags_combo_box_model);
    this->ui->cb_tags->setModelColumn(this->_tags_combo_box_model->record().indexOf(COLUMN_TAG));
}

void RelationView::initialQuestionTagsListView(){
    if(this->_selected_question == nullptr){
        return;
    }

    QStringListModel *gm_model = new QStringListModel;
    QList<QString> tags;

    this->ui->lv_question_tags->setModel(gm_model);
    for(Tag *tag : this->_selected_question->get_tags()){
        tags.append(tag->get_tag());
    }
    gm_model->setStringList(tags);
}

void RelationView::on_cb_tags_currentIndexChanged(int index){
    int id, id_column_index, tag_column_index;
    QString tag;

    id_column_index = this->_tags_combo_box_model->record().indexOf(COLUMN_ID);
    tag_column_index = this->_tags_combo_box_model->record().indexOf(COLUMN_TAG);

    id = this->_tags_combo_box_model->index(index, id_column_index).data(Qt::DisplayRole).toInt();
    tag = this->_tags_combo_box_model->index(index, tag_column_index).data(Qt::DisplayRole).toString();

    this->_selected_cb = new Tag(id, tag, this);
}

void RelationView::on_lv_questions_clicked(const QModelIndex &index){
    int id, id_column_index, value_column_index, answer_column_index;
    QString value, answer;

    id_column_index = this->_questions_table_model->record().indexOf(COLUMN_ID);
    value_column_index = this->_questions_table_model->record().indexOf(COLUMN_VALUE);
    answer_column_index = this->_questions_table_model->record().indexOf(COLUMN_ANSWER);

    id = this->_questions_table_model->index(index.row(), id_column_index).data(Qt::DisplayRole).toInt();
    value = this->_questions_table_model->index(index.row(), value_column_index).data(Qt::DisplayRole).toString();
    answer = this->_questions_table_model->index(index.row(), answer_column_index).data(Qt::DisplayRole).toString();

    this->_selected_question = new Question(id, value, answer, {});
    this->_selected_question->getAllRelated();
    initialQuestionTagsListView();
}

void RelationView::on_lv_question_tags_clicked(const QModelIndex &index){
    this->_from_quest = this->_selected_question->get_tags().at(index.row());
}

void RelationView::on_b_create_relation_clicked(){
    if(this->_selected_cb == nullptr){
        this->printInfo(TAG_FROM_COMBO_BOX_SHOULD_BE_SELECTED, true);
        return;
    }

    if(this->_selected_question == nullptr){
        this->printInfo(SELECT_QUESTION_FIRST);
        return;
    }
    //TODO: Validate is relation is not EXIST.

    if(this->_selected_cb->isRelationCreated(this->_selected_question)){
      this->_questions_table_model->select();
      this->printInfo(RELATION_QUESTION_WIT_TAG_CREATED);
    } else {
      this->printInfo(DATABASE_ERROR, true);
    }

    emit create_relation();
}

void RelationView::on_b_remove_relation_clicked(){
  if(this->_from_quest == nullptr){
    this->printInfo(SELECT_TAG_FROM_QUESTION, true);
    return;
  }

  if(this->_selected_question == nullptr){
    this->printInfo(SELECT_QUESTION_FIRST, true);
    return;
  }

  if(this->_from_quest->isRemovedRelation(this->_selected_question)){
    this->_questions_table_model->select();
    this->printInfo(REMOVED_RELATION_QUESTION_AND_TAG_SUCCESFULLY);
  } else {
    printInfo(DATABASE_ERROR, true);
  }

  emit remove_relation();
}

void RelationView::added_question_to_db(){
  this->initialQuestionsListView();
}

void RelationView::remove_question_from_db(){
  this->initialQuestionsListView();
}

void RelationView::update_question_from_db(){
  this->initialQuestionsListView();
}

void RelationView::added_tag_to_db(){
  this->initialTagsComboBox();
}

void RelationView::remove_tag_from_db(){
  this->initialTagsComboBox();
}

void RelationView::update_tag_from_db(){
  this->initialTagsComboBox();
}
