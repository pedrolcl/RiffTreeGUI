// Copyright (C) 2025-2026 Pedro López-Cabanillas
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QPixmap>
#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>

#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("About RiffTreeGUI"));
    resize(420, 300);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QTabWidget *tabs = new QTabWidget(this);
    QWidget *aboutTab = new QWidget(this);
    QVBoxLayout *aboutLayout = new QVBoxLayout(aboutTab);

    QHBoxLayout *logoLayout = new QHBoxLayout();
    QLabel *logoLabel = new QLabel();
    QPixmap logo(":/images/RiffTree.png");
    if (!logo.isNull()) {
        logoLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        logoLabel->setPixmap(
            logo.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    QLabel *aboutText =
        new QLabel(tr("<h2>RiffTreeGUI v%1</h2>"
                      "<p>RiffTreeGUI is a RIFF file structure viewer with hex content view</p>"
                      "<p>Copyright © 2025-2026 Pedro López-Cabanillas &lt;plcl@users.sf.net&gt;</p>"
                      "<a href='https://github.com/pedrolcl/RiffTreeGUI/'>https://github.com/pedrolcl/RiffTreeGUI</a>"
                      "<p>Built with Qt %2</p>").arg(QT_STRINGIFY(VERSION), QT_VERSION_STR));
    aboutText->setWordWrap(true);
    aboutText->setOpenExternalLinks(true);

    logoLayout->addWidget(logoLabel);
    logoLayout->addWidget(aboutText);
    aboutLayout->addLayout(logoLayout);
    aboutLayout->addStretch();
    tabs->addTab(aboutTab, tr("About"));

    QWidget *creditsTab = new QWidget(this);
    QVBoxLayout *creditsLayout = new QVBoxLayout(creditsTab);
    QLabel *creditsLabel =
        new QLabel(tr("This has been possible thanks to the following projects:<br><br>"
                      "<b>rifftree (Apache 2.0 license)</b><br>"
                      "<a href='https://github.com/jesustorresdev/rifftree'>https://github.com/jesustorresdev/rifftree</a><br>"
                      "Copyright © 2013 Jesús Torres<br><br>"
                      "<b>QHexView (MIT license)</b><br>"
                      "<a href='https://github.com/Dax89/QHexView'>https://github.com/Dax89/QHexView</a><br>"
                      "Copyright © 2014 Dax89<br>"));

    creditsLabel->setWordWrap(true);
    creditsLabel->setOpenExternalLinks(true);
    creditsLayout->addWidget(creditsLabel);
    creditsLayout->addStretch();
    tabs->addTab(creditsTab, tr("Credits"));

    QWidget *licenseTab = new QWidget(this);
    QVBoxLayout *licenseLayout = new QVBoxLayout(licenseTab);
    QLabel *licenseText = new QLabel(tr("<p>Copyright © 2025-2026 Pedro López-Cabanillas</p>"
    "<p>This program is free software: you can redistribute it and/or modify "
    "it under the terms of the GNU General Public License as published by "
    "the Free Software Foundation, either version 3 of the License, or "
    "(at your option) any later version.</p>"
    "<p>This program is distributed in the hope that it will be useful, "
    "but WITHOUT ANY WARRANTY; without even the implied warranty of "
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
    "GNU General Public License for more details.</p>"
    "<p>You should have received a copy of the GNU General Public License "
    "along with this program. If not, see "
    "<a href='https://www.gnu.org/licenses'>https://www.gnu.org/licenses</a></p>"));
    licenseText->setOpenExternalLinks(true);
    licenseText->setWordWrap(true);
    licenseLayout->addWidget(licenseText);
    tabs->addTab(licenseTab, tr("License"));

    mainLayout->addWidget(tabs);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    mainLayout->addWidget(buttonBox);
}
