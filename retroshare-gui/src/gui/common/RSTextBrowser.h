#ifndef RSTEXTBROWSER_H
#define RSTEXTBROWSER_H

#include <QTextBrowser>
#include "util/RsSyntaxHighlighter.h"

class RSImageBlockWidget;

class RSTextBrowser : public QTextBrowser
{
	Q_OBJECT

	Q_PROPERTY(QColor textColorQuote READ textColorQuote WRITE setTextColorQuote)

public:
	explicit RSTextBrowser(QWidget *parent = 0);

	void setPlaceholderText(const QString &text);
	void setImageBlockWidget(RSImageBlockWidget *widget);
	void resetImagesStatus(bool load);

	void activateLinkClick(bool active);

	QVariant loadResource(int type, const QUrl &name) override;

	QColor textColorQuote() const { return highliter->textColorQuote();}

public slots:
	void showImages();
	void setTextColorQuote(QColor textColorQuote) { highliter->setTextColorQuote(textColorQuote);}

private slots:
	void linkClicked(const QUrl &url);
	void destroyImageBlockWidget();

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	QString mPlaceholderText;
	bool mShowImages;
	RSImageBlockWidget *mImageBlockWidget;
	bool mLinkClickActive;
	RsSyntaxHighlighter *highliter;
};

#endif // RSTEXTBROWSER_H
