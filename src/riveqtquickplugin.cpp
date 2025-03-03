
// SPDX-FileCopyrightText: 2023 Jeremias Bosch <jeremias.bosch@basyskom.com>
// SPDX-FileCopyrightText: 2023 basysKom GmbH
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "riveqtquickplugin.h"

#include "qqml.h"
#include "qqmlengine.h"
#include "qtquick/riveqtquickitem.h"
#include "qtquick/riveqtstatemachineinputmap.h"

void RiveQtQuickPlugin::registerTypes(const char *uri)
{
  qmlRegisterType<RiveQtQuickItem>("RiveQtQuickPlugin", 1, 0, "RiveQtQuickItem");
  qRegisterMetaType<AnimationInfo>("AnimationInfo");
  qRegisterMetaType<QVector<AnimationInfo>>("QVector<AnimationInfo>");

  qRegisterMetaType<ArtBoardInfo>("ArtBoardInfo");
  qRegisterMetaType<QVector<ArtBoardInfo>>("QVector<ArtBoardInfo>");

  qRegisterMetaType<StateMachineInfo>("StateMachineInfo");
  qRegisterMetaType<QVector<StateMachineInfo>>("QVector<StateMachineInfo>");
  qRegisterMetaType<RiveQtStateMachineInputMap *>("RiveQtStateMachineInputMap*");
}
