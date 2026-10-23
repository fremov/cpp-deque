#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>

#include <algorithm>
#include <functional>
#include <iterator>

#include "algo.h"

namespace {
constexpr size_t kMaxSize = 1000;

const Model::Deque kTea {
    "Чай Лунцзин",
    "Эрл Грей",
    "Сенча",
    "Пуэр",
    "Дарджилинг",
    "Ассам",
    "Матча",
    "Ганпаудер",
    "Оолонг",
    "Лапсанг Сушонг"
};

const Model::Deque kCakes {
    "Красный бархат",
    "Наполеон",
    "Медовик",
    "Тирамису",
    "Прага",
    "Чизкейк",
    "Захер",
    "Эстерхази",
    "Морковный торт",
    "Чёрный лес"
};
}  // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ApplyModel();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SetRandomGen(const std::mt19937& random_gen) {
    random_gen_ = random_gen;
}

void MainWindow::ApplyModel() {
    auto preserve_iter = deque_model_.iterator;

    ui->list_widget->clear();
    for (size_t i = 0; i < deque_model_.items.size(); ++i) {
        ui->list_widget->addItem(QString("%1: %2").arg(i).arg(QString::fromStdString(deque_model_.items[i])));
    }
    ui->list_widget->addItem("end");

    deque_model_.iterator = preserve_iter;

    const bool is_empty = deque_model_.items.empty();
    ui->btn_pop_front->setEnabled(!is_empty);
    ui->btn_pop_back->setEnabled(!is_empty);
    ui->btn_erase->setEnabled(!is_empty);
    ui->txt_size->setText(QString::number(deque_model_.items.size()));

    ApplyIterator();
}

void MainWindow::ApplyIterator() {
    const auto offset = std::distance(deque_model_.items.begin(), deque_model_.iterator);
    ui->list_widget->setCurrentRow(static_cast<int>(offset));

    const bool is_end = deque_model_.iterator == deque_model_.items.end();
    ui->btn_edit->setEnabled(!is_end);
    ui->btn_erase->setEnabled(!is_end);
    ui->btn_inc_iterator->setEnabled(!is_end);
    ui->btn_dec_iterator->setEnabled(deque_model_.iterator != deque_model_.items.begin());

    ui->txt_elem_content->setText(is_end ? QString{} : QString::fromStdString(*deque_model_.iterator));
}

void MainWindow::on_list_widget_currentRowChanged(int currentRow) {
    if (currentRow < 0) {
        return;
    }

    const auto size = static_cast<int>(deque_model_.items.size());
    const auto new_iter = deque_model_.items.begin() + std::min(currentRow, size);
    if (new_iter == deque_model_.iterator) {
        return;
    }

    deque_model_.iterator = new_iter;
    ApplyIterator();
}

void MainWindow::on_btn_clear_clicked() {
    deque_model_.items.clear();
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_push_front_clicked() {
    deque_model_.items.push_front(ui->txt_elem_content->text().toStdString());
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_push_back_clicked() {
    deque_model_.items.push_back(ui->txt_elem_content->text().toStdString());
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_pop_front_clicked() {
    if (!deque_model_.items.empty()) {
        deque_model_.items.pop_front();
    }
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_pop_back_clicked() {
    if (!deque_model_.items.empty()) {
        deque_model_.items.pop_back();
    }
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_insert_clicked() {
    deque_model_.items.insert(deque_model_.iterator, ui->txt_elem_content->text().toStdString());
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_erase_clicked() {
    if (deque_model_.iterator == deque_model_.items.end()) {
        return;
    }

    deque_model_.items.erase(deque_model_.iterator);
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_edit_clicked() {
    if (deque_model_.iterator == deque_model_.items.end()) {
        return;
    }

    *deque_model_.iterator = ui->txt_elem_content->text().toStdString();
    ApplyModel();
}

void MainWindow::on_btn_begin_clicked() {
    deque_model_.iterator = deque_model_.items.begin();
    ApplyIterator();
}

void MainWindow::on_btn_end_clicked() {
    deque_model_.iterator = deque_model_.items.end();
    ApplyIterator();
}

void MainWindow::on_btn_dec_iterator_clicked() {
    if (deque_model_.iterator == deque_model_.items.begin()) {
        return;
    }

    --deque_model_.iterator;
    ApplyIterator();
}

void MainWindow::on_btn_inc_iterator_clicked() {
    if (deque_model_.iterator == deque_model_.items.end()) {
        return;
    }

    ++deque_model_.iterator;
    ApplyIterator();
}

void MainWindow::on_btn_find_clicked() {
    const auto value = ui->txt_elem_content->text().toStdString();
    deque_model_.iterator = std::find(deque_model_.items.begin(), deque_model_.items.end(), value);
    ApplyIterator();
}

void MainWindow::on_btn_count_clicked() {
    const auto value = ui->le_count->text().toStdString();
    const auto count = std::count(deque_model_.items.begin(), deque_model_.items.end(), value);
    ui->lbl_count->setText(QString::number(count));
}

void MainWindow::on_btn_min_element_clicked() {
    deque_model_.iterator = std::min_element(deque_model_.items.begin(), deque_model_.items.end());
    ApplyIterator();
}

void MainWindow::on_btn_max_element_clicked() {
    deque_model_.iterator = std::max_element(deque_model_.items.begin(), deque_model_.items.end());
    ApplyIterator();
}

void MainWindow::on_btn_sort_clicked() {
    deque_model_.items = MergeSort(deque_model_.items, std::less<std::string>{});
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_sOrT_clicked() {
    const auto case_insensitive_less = [](const std::string& lhs, const std::string& rhs) {
        return QString::compare(QString::fromStdString(lhs), QString::fromStdString(rhs),
                                Qt::CaseInsensitive) < 0;
    };
    deque_model_.items = MergeSort(deque_model_.items, case_insensitive_less);
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_shuffle_clicked() {
    std::shuffle(deque_model_.items.begin(), deque_model_.items.end(), random_gen_);
    ApplyModel();
}

void MainWindow::on_btn_unique_clicked() {
    auto& items = deque_model_.items;
    if (!std::is_sorted(items.begin(), items.end())) {
        return;
    }

    items.erase(std::unique(items.begin(), items.end()), items.end());
    deque_model_.iterator = items.begin();
    ApplyModel();
}

void MainWindow::on_btn_reverse_clicked() {
    std::reverse(deque_model_.items.begin(), deque_model_.items.end());
    ApplyModel();
}

void MainWindow::on_btn_lower_bound_clicked() {
    auto& items = deque_model_.items;
    if (!std::is_sorted(items.begin(), items.end())) {
        return;
    }

    const auto value = ui->txt_elem_content->text().toStdString();
    deque_model_.iterator = std::lower_bound(items.begin(), items.end(), value);
    ApplyIterator();
}

void MainWindow::on_btn_upper_bound_clicked() {
    auto& items = deque_model_.items;
    if (!std::is_sorted(items.begin(), items.end())) {
        return;
    }

    const auto value = ui->txt_elem_content->text().toStdString();
    deque_model_.iterator = std::upper_bound(items.begin(), items.end(), value);
    ApplyIterator();
}

void MainWindow::on_btn_resize_clicked() {
    bool ok = false;
    const auto new_size = ui->txt_size->text().toUInt(&ok);
    if (ok && new_size <= kMaxSize) {
        deque_model_.items.resize(new_size);
    }
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_tea_clicked() {
    deque_model_.items = kTea;
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_cakes_clicked() {
    deque_model_.items = kCakes;
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}
