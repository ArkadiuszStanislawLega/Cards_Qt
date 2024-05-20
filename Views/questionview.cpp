#include "questionview.h"

void QuestionView::initialQuestionsListView() {
  this->_table_model = new QSqlRelationalTableModel;
  this->_table_model->setTable(TABLE_QUESTIONS);
  this->_table_model->setSort(
      this->_table_model->record().indexOf(COLUMN_VALUE), Qt::AscendingOrder);
  this->_table_model->select();

  this->ui->lv_created_quesions->setModel(this->_table_model);
  this->ui->lv_created_quesions->setModelColumn(
      this->_table_model->record().indexOf(COLUMN_VALUE));
}

void QuestionView::initialTagsComboBoxData() {
  this->_cb_tags_model = new QSqlRelationalTableModel;
  this->_cb_tags_model->setTable(TABLE_TAGS);
  this->_cb_tags_model->select();
  this->ui->cb_tags->setModel(this->_cb_tags_model);
  this->ui->cb_tags->setModelColumn(
      this->_cb_tags_model->record().indexOf(COLUMN_TAG));
}

void QuestionView::cleanTextEditors() {
  this->ui->te_value->clear();
  this->ui->te_answer->clear();
}

void QuestionView::printInfo(const QString &value, bool isError = false) {
  QPalette pal = this->ui->l_info->palette();
  pal.setColor(QPalette::Window, QColor(isError ? Qt::red : Qt::transparent));

  this->ui->l_info->setText(value);
  this->ui->l_info->setPalette(pal);
}

QuestionView::QuestionView(QWidget *parent)
    : QWidget{parent}, ui(new Ui::QuestionView) {
  this->ui->setupUi(this);
  this->_selected_question = new Question(this);
  this->initialQuestionsListView();
  this->initialTagsComboBoxData();
}

void QuestionView::on_b_create_question_clicked() {
  if (ui->te_answer->toPlainText().isEmpty()) {
    this->printInfo(FIELD_ANSWER_CANT_EMPTY, true);
    return;
  }

  if (ui->te_value->toPlainText().isEmpty()) {
    this->printInfo(FIELD_VALUE_CANT_EMPTY, true);
    return;
  }

  this->createRelationQuestionAndTag(addQuestionToDb());
  this->cleanTextEditors();

  emit added_question_to_db();
}

///
/// \brief QuestionView::addQuestionToDb Create question in database.
/// \return Created question with Id from database.
///
Question *QuestionView::addQuestionToDb() {
  Question *q = new Question(this);
  q->setAnswer(ui->te_answer->toPlainText());
  q->setValue(ui->te_value->toPlainText());
  q->setIsActive(ui->rb_isActive->isChecked());

  QuestionModelSql sqlModel = QuestionModelSql(q, this);

  try {
    if (sqlModel.isInsertedSql()) {
      this->_table_model->select();
      this->cleanTextEditors();
      this->printInfo(QUESTION_CREATED_CORRECTLY);
    }
  } catch (std::invalid_argument &e) {
    this->printInfo(DATABASE_ERROR, true);
    qWarning() << "QuestionView::addQuestionToDb" << e.what();
  }

  return q;
}

void QuestionView::createRelationQuestionAndTag(Question *q) {
  if (!q) {
    throw std::invalid_argument("QuestionView::createRelationQuestionAndTag -- "
                                "pointer to question is empty.");
  }

  if (!this->_selected_tag) {
    throw std::invalid_argument("QuestionView::createRelationQuestionAndTag -- "
                                "pointer to tag is empty.");
  }

  if (q->getId() <= 0) {
    throw std::invalid_argument("QuestionView::createRelationQuestionAndTag -- "
                                "porperty id in quesiton is zero or subzero.");
  }

  if (this->_selected_tag->getId() <= 0) {
    throw std::invalid_argument("QuestionView::createRelationQuestionAndTag -- "
                                "property id in quesiton is zer or subzero");
  }

  if (q && this->_selected_tag) {
    TagAndQuestionRelationSql relation =
        TagAndQuestionRelationSql(this->_selected_tag, q, this);
    relation.isInsertedSql();
    delete q;
  }
}

void QuestionView::on_b_update_question_clicked() {
  if (!this->_selected_question) {
    this->printInfo(INFO_FIRST_SELECT_QUESTION, true);
    return;
  }

  if (ui->te_answer->toPlainText().isEmpty()) {
    this->printInfo(FIELD_ANSWER_CANT_EMPTY, true);
    return;
  }

  if (ui->te_value->toPlainText().isEmpty()) {
    this->printInfo(FIELD_VALUE_CANT_EMPTY, true);
    return;
  }

  this->_selected_question->setAnswer(this->ui->te_answer->toPlainText());
  this->_selected_question->setValue(this->ui->te_value->toPlainText());
  this->_selected_question->setIsActive(this->ui->rb_isActive->isChecked());

  QuestionModelSql sqlModel = QuestionModelSql(this->_selected_question, this);

  try {
    if (sqlModel.updateSql()) {
      this->_table_model->select();
      this->printInfo(QUESTION_UPDATED);
      this->cleanTextEditors();
    }
    emit update_question_from_db();
  } catch (std::invalid_argument &e) {
    this->printInfo(DATABASE_ERROR, true);
    qWarning() << "QuestionView::on_b_update_question_clicked" << e.what();
  }
}

void QuestionView::on_b_remove_question_clicked() {
  if (this->_selected_question == nullptr) {
    this->printInfo(INFO_FIRST_SELECT_QUESTION, true);
    return;
  }

  if (this->_selected_question->getId() <= 0) {
    throw std::invalid_argument("QuestionView::on_b_remove_question_clicked -- "
                                "id property in question is zero or subzero.");
  }

  QuestionModelSql sqlModel = QuestionModelSql(this->_selected_question, this);
  TagAndQuestionRelationSql relation =
      TagAndQuestionRelationSql(new Tag(this), this->_selected_question, this);

  try {
    if (relation.isAllRelationRemoved() && sqlModel.isDeleteSql()) {
      this->_selected_question = nullptr;
      this->_table_model->select();
      this->cleanTextEditors();
      this->printInfo(QUESTION_SUCCESFULLY_REMOVED);
    }
    emit remove_question_from_db();
  } catch (std::invalid_argument &e) {
    this->printInfo(DATABASE_ERROR, true);
    qWarning() << "QuestionView::on_b_remove_question_clicked" << e.what();
  }
}

void QuestionView::on_lv_created_quesions_pressed(const QModelIndex &index) {

  int id, id_column_index, value_column_index, answer_column_index,
      is_active_column_index;

  QString value, answer;
  bool isActive;

  id_column_index = this->_table_model->record().indexOf(COLUMN_ID);
  value_column_index = this->_table_model->record().indexOf(COLUMN_VALUE);
  answer_column_index = this->_table_model->record().indexOf(COLUMN_ANSWER);

  is_active_column_index =
      this->_table_model->record().indexOf(COLUMN_IS_ACTIVE);

  id = this->_table_model->index(index.row(), id_column_index)
           .data(Qt::DisplayRole)
           .toInt();
  value = this->_table_model->index(index.row(), value_column_index)
              .data(Qt::DisplayRole)
              .toString();
  answer = this->_table_model->index(index.row(), answer_column_index)
               .data(Qt::DisplayRole)
               .toString();

  isActive = this->_table_model->index(index.row(), is_active_column_index)
                 .data(Qt::DisplayRole)
                 .toBool();

  try {

    this->_selected_question = new Question(id, value, answer, isActive, {});
    TagAndQuestionRelationSql *relation =
        new TagAndQuestionRelationSql(nullptr, this->_selected_question, this);

    this->_selected_question->setTags(relation->getRelatedTags());
    this->_selected_question->setParent(this);
  } catch (std::invalid_argument &e) {
    qWarning() << "QuestionView::on_lv_created_quesions_pressed" << e.what();
    this->printInfo(DATABASE_ERROR, true);
  }

  this->ui->te_answer->setText(answer);
  this->ui->te_value->setText(value);
  this->ui->rb_isActive->setChecked(isActive);
}

void QuestionView::on_cb_tags_currentIndexChanged(int index) {
  int id, id_column, tag_column_index;
  QString tag;

  id_column = this->_cb_tags_model->record().indexOf(COLUMN_ID);
  tag_column_index = this->_cb_tags_model->record().indexOf(COLUMN_TAG);

  id = this->_cb_tags_model->index(index, id_column)
           .data(Qt::DisplayRole)
           .toInt();
  tag = this->_cb_tags_model->index(index, tag_column_index)
            .data(Qt::DisplayRole)
            .toString();

  this->_selected_tag = new Tag(id, tag, this);
}

void QuestionView::on_le_searching_bar_textChanged(const QString &arg1) {
  this->_table_model->setFilter(TABLE_QUESTIONS + "." + COLUMN_VALUE +
                                " LIKE \"%" + arg1 + "%\"");
  this->_table_model->select();
}

void QuestionView::added_tag_to_db() { this->_cb_tags_model->select(); }
