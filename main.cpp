#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtGui/QPixmap>
#include <QtGui/QMouseEvent>
#include <QtCore/QFile>
#include <QtCore/QPoint>
#include <vector>

class MemoryViewer : public QWidget {
public:
    MemoryViewer(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowFlags(Qt::FramelessWindowHint);
        setAttribute(Qt::WA_TranslucentBackground);
        setFixedSize(520, 720);

        quotes = {
            "Кожен раз, коли я дивлюся на тебе, я розумію, що серце не брехало.",
            "Ти — найкраще, що могло зі мною статися. Найкраще й назавжди.",
            "Поруч із тобою навіть мовчання звучить, як найніжніша музика.",
            "Я хочу прокидатися щодня і знати, що ти є у моєму світі.",
            "Твоя посмішка — це той маленький всесвіт, у якому я хочу жити.",
            "З тобою найзвичайніші миті стають найдорогоціннішими спогадами.",
            "Ти навчила моє серце битися в потрібному ритмі.",
            "Кохати тебе — найлегше і найприродніше, що я будь-коли робив.",
            "Ти — мій дім. Де б я не був, я завжди повертаюся до тебе.",
            "Дякую тобі за кожен момент, коли ти обирала бути поруч зі мною."
        };

        setupUI();
        loadPhotos();
        showCurrent();
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) m_lastPos = event->globalPosition().toPoint();
    }
    void mouseMoveEvent(QMouseEvent *event) override {
        if (event->buttons() & Qt::LeftButton) {
            move(pos() + (event->globalPosition().toPoint() - m_lastPos));
            m_lastPos = event->globalPosition().toPoint();
        }
    }

private:
    std::vector<QString> quotes;
    std::vector<QString> photoPaths;
    int currentIndex = 0;
    QPoint m_lastPos;

    QLabel *photoLabel, *quoteLabel, *counterLabel;
    QPushButton *nextBtn, *restartBtn, *exitBtn;
    QHBoxLayout *finalBtnLayout;

    void setupUI() {
        auto *layout = new QVBoxLayout(this);
        auto *mainFrame = new QFrame();
        mainFrame->setStyleSheet(
            "QFrame { background-color: #FFF0F5; border-radius: 20px; }"
            "QPushButton { border-radius: 20px; font-weight: bold; font-size: 14px; }"
            "QPushButton#nextBtn { background-color: #F4A7B9; color: white; border-radius: 22px; font-size: 15px; }"
            "QPushButton#nextBtn:hover { background-color: #E87D9A; }"
            "QPushButton#restartBtn { background-color: #A7C7F4; color: white; }"
            "QPushButton#restartBtn:hover { background-color: #8DB5E8; }"
            "QPushButton#exitBtn { background-color: #F4A7B9; color: white; }"
            "QPushButton#exitBtn:hover { background-color: #E87D9A; }"
            "QLabel#quote { color: #C06080; font-size: 16px; qproperty-alignment: AlignCenter; }"
        );

        auto *contentLayout = new QVBoxLayout(mainFrame);
        auto *closeBtn = new QPushButton("✕", mainFrame);
        closeBtn->setFixedSize(32, 32);
        closeBtn->setStyleSheet("background: transparent; color: #C07090; font-size: 18px;");
        connect(closeBtn, &QPushButton::clicked, this, &QWidget::close);

        photoLabel = new QLabel();
        photoLabel->setAlignment(Qt::AlignCenter);
        counterLabel = new QLabel();
        counterLabel->setAlignment(Qt::AlignCenter);
        counterLabel->setStyleSheet("color: #D4879A; font-size: 12px;");

        quoteLabel = new QLabel();
        quoteLabel->setObjectName("quote");
        quoteLabel->setWordWrap(true);
        quoteLabel->setContentsMargins(20, 0, 20, 0);

        // Кнопки
        nextBtn = new QPushButton("✨ Наступний спогад");
        nextBtn->setObjectName("nextBtn");
        nextBtn->setFixedHeight(45);
        connect(nextBtn, &QPushButton::clicked, this, &MemoryViewer::nextMemory);

        // Фінальні кнопки (спочатку приховані)
        restartBtn = new QPushButton("🔄 Ще раз");
        restartBtn->setObjectName("restartBtn");
        restartBtn->setFixedSize(150, 40);
        restartBtn->hide();
        connect(restartBtn, &QPushButton::clicked, this, &MemoryViewer::restart);

        exitBtn = new QPushButton("🎀 Обійняти");
        exitBtn->setObjectName("exitBtn");
        exitBtn->setFixedSize(150, 40);
        exitBtn->hide();
        connect(exitBtn, &QPushButton::clicked, this, &QWidget::close);

        finalBtnLayout = new QHBoxLayout();
        finalBtnLayout->addWidget(restartBtn);
        finalBtnLayout->addWidget(exitBtn);
        finalBtnLayout->setAlignment(Qt::AlignCenter);

        contentLayout->addWidget(closeBtn, 0, Qt::AlignRight);
        contentLayout->addWidget(photoLabel, 1);
        contentLayout->addWidget(counterLabel);
        contentLayout->addWidget(quoteLabel);
        contentLayout->addSpacing(10);
        contentLayout->addWidget(nextBtn);
        contentLayout->addLayout(finalBtnLayout);
        contentLayout->setContentsMargins(30, 10, 30, 30);

        layout->addWidget(mainFrame);
    }

    void loadPhotos() {
        for (int i = 1; i <= 10; ++i) {
            QString path = QString::number(i) + ".png";
            if (QFile::exists(path)) photoPaths.push_back(path);
        }
    }

    void showCurrent() {
        if (photoPaths.empty()) return;
        QPixmap pix(photoPaths[currentIndex]);
        photoLabel->setPixmap(pix.scaled(400, 380, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        quoteLabel->setText("❝ " + quotes[currentIndex % quotes.size()] + " ❞");
        counterLabel->setText(QString("Мить %1 із %2").arg(currentIndex + 1).arg(photoPaths.size()));
    }

    void nextMemory() {
        if (currentIndex < (int)photoPaths.size() - 1) {
            currentIndex++;
            showCurrent();
        } else {
            showFinalScreen();
        }
    }

    void showFinalScreen() {
        nextBtn->hide();
        restartBtn->show();
        exitBtn->show();
        counterLabel->setText("Наші спогади нескінченні... ❤️");
        quoteLabel->setText("❝ Я кохаю тебе понад усе на світі ❞");
    }

    void restart() {
        currentIndex = 0;
        restartBtn->hide();
        exitBtn->hide();
        nextBtn->show();
        showCurrent();
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MemoryViewer viewer;
    viewer.show();
    return app.exec();
}
