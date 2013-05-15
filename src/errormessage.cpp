/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include <QGraphicsLinearLayout>
#include <QApplication>

#include <MLabel>
#include <MButton>

#include "errormessage.h"
#include "preferences.h"

ErrorMessage::ErrorMessage()
    : MDialog("", M::OkButton)
{
    MWidget *centralWidget = new MWidget;
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);

    m_messageText = new MLabel(this);
    m_messageText->setWordWrap(true);

    m_checkBox = new MButton(this);
    m_checkBox->setViewType(MButton::checkboxType);
    m_checkBox->setCheckable(true);
    m_checkBoxLabel = new MLabel();
    m_checkBoxLabel->setWordWrap(true);
    m_checkBoxLabel->setTextElide(true);
    
    //% "Show this message again"
    m_checkBoxLabel->setText(qtTrId("meegovideo_show_this_message_again"));

    QGraphicsLinearLayout *checkboxLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    checkboxLayout->addItem(m_checkBox);
    checkboxLayout->addItem(m_checkBoxLabel);
    checkboxLayout->setAlignment(m_checkBox, Qt::AlignCenter);
    checkboxLayout->setAlignment(m_checkBoxLabel, Qt::AlignCenter);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    layout->addItem(m_messageText);
    layout->addItem(checkboxLayout);
    
    //% "%1 - Error"
    setTitle(qtTrId("meegovideo_app_error").arg(qApp->applicationName()));
}

void ErrorMessage::showMessage(const QString &message)
{
    bool checked = Preferences::isDisplayMPlayerMessageEnabled();
    if (checked) {
        m_checkBox->setChecked(checked);
        m_messageText->setText(message);
        exec();
    }
}

void ErrorMessage::done(int result)
{
    if (result == MDialog::Accepted) {
        const bool checked = m_checkBox->isChecked();
        Preferences::setDisplayMPlayerMessageEnabled(checked);
    }

    MDialog::done(result);
}
