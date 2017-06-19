#include <qapplication.h>
#include <qlabel.h>
#include <qlayout.h>

#include "LayoutWindow.moc"

LayoutWindow::LayoutWindow(QWidget *parent, const char *name) : QMainWindow(parent, name)
{
  this->setCaption("Layouts");
  QWidget *widget = new QWidget(this);
  setCentralWidget(widget);
  
  QHBoxLayout *horizontal = new QHBoxLayout(widget);
  QVBoxLayout *vertical = new QVBoxLayout();
  QLabel* label1 = new QLabel("Top", centralWidget(), "textLabel1" );
  QLabel* label2 = new QLabel("Bottom", widget, "Label 2");
  QLabel* label3 = new QLabel("Right",widget, "Label 3");

  vertical->addWidget(label1);
  vertical->addWidget(label2);
  horizontal->addLayout(vertical);
  horizontal->addWidget(label3); 
  resize( 150, 100 );
}

LayoutWindow::~LayoutWindow()
{
}

int main(int argc, char **argv)
{
  QApplication app(argc,argv);
  LayoutWindow *window = new LayoutWindow();

  app.setMainWidget(window);
  window->show();

  return app.exec();
}

