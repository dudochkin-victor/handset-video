/*
 * Meego-handset-video is an video player for MeeGo handset.
 * Copyright (C) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include <MDialog>

class Preferences;
class MLabel;
class MButton;

///
/// @class ErrorMessage
/// @brief ErrorMessage Provide an error message display dialog that can remember its state across sessions
///
/// This class displays a dialog that displays error text and a check box that
/// allows the user to control the dialog display in future. If the check box is checked,
/// the dialog will be displayed again when the showMessage method is called. If
/// the check box has been unchecked, the next time
/// ErrorMessage::showMessage(const QString &) method is called the dialog
/// won't be displayed.
///
/// The class uses the Preferences class internally to store the dialog state and
/// saves the state to the \"MPlayerMessageEnabled\" field in settings.
///

class ErrorMessage: public MDialog
{
    Q_OBJECT
public:
    /// @brief Constructor
    /// @param parent QWidget Optional parent widget.
    ErrorMessage();

    /// @brief Display error message
    /// This method shows error message by executing QDialog::exec(). Appropriately
    /// the dialog will be displayed as a modal dialog, blocking until the user closes it.
    /// @param message The message to be displayed in the message dialog
    void showMessage(const QString & message);

public slots:
    virtual void done(int result);

private:
    MLabel *m_messageText;
    MButton *m_checkBox;
    MLabel *m_checkBoxLabel;
};

#endif // ERRORMESSAGE_H
