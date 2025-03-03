
// SPDX-FileCopyrightText: 2023 Jeremias Bosch <jeremias.bosch@basyskom.com>
// SPDX-FileCopyrightText: 2023 basysKom GmbH
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <QElapsedTimer>
#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QSGRenderNode>
#include <QSGTextureProvider>

#include <QImage>
#include <QPainter>

#include <QTimer>
#include "rive/artboard.hpp"
#include "riveqtfactory.h"
#include "riveqtstatemachineinputmap.h"
#include "rive/listener_type.hpp"

class RiveQSGRenderNode;

class RiveQSGRHIRenderNode;

// TODO: Move Structs in extra file
struct AnimationInfo
{
  Q_GADGET

  Q_PROPERTY(int id MEMBER id CONSTANT)
  Q_PROPERTY(QString name MEMBER name CONSTANT)
  Q_PROPERTY(float duration MEMBER duration CONSTANT)
  Q_PROPERTY(uint32_t fps MEMBER fps CONSTANT)

public:
  int id;
  QString name;
  float duration;
  uint32_t fps;
};

class ArtBoardInfo
{
  Q_GADGET

  Q_PROPERTY(int id MEMBER id CONSTANT)
  Q_PROPERTY(QString name MEMBER name CONSTANT)

public:
  int id;
  QString name;
};

struct StateMachineInfo
{
  Q_GADGET

  Q_PROPERTY(int id MEMBER id CONSTANT)
  Q_PROPERTY(QString name MEMBER name CONSTANT)

public:
  int id;
  QString name;
};

class RiveQtQuickItem : public QQuickItem
{
  Q_OBJECT

  Q_PROPERTY(QString fileSource READ fileSource WRITE setFileSource NOTIFY fileSourceChanged)
  Q_PROPERTY(LoadingStatus loadingStatus READ loadingStatus NOTIFY loadingStatusChanged)
  Q_PROPERTY(QVector<ArtBoardInfo> artboards READ artboards NOTIFY artboardsChanged)
  Q_PROPERTY(QVector<AnimationInfo> animations READ animations NOTIFY animationsChanged)
  Q_PROPERTY(QVector<StateMachineInfo> stateMachines READ stateMachines NOTIFY stateMachinesChanged)

  Q_PROPERTY(int currentArtboardIndex READ currentArtboardIndex WRITE setCurrentArtboardIndex NOTIFY currentArtboardIndexChanged)
  Q_PROPERTY(
    int currentStateMachineIndex READ currentStateMachineIndex WRITE setCurrentStateMachineIndex NOTIFY currentStateMachineIndexChanged)

  Q_PROPERTY(bool interactive READ interactive WRITE setInteractive NOTIFY interactiveChanged)

  Q_PROPERTY(RiveQtStateMachineInputMap *stateMachineInterface READ stateMachineInterface NOTIFY stateMachineInterfaceChanged)
public:
  enum LoadingStatus
  {
    Idle,
    Loading,
    Loaded,
    Error
  };
  Q_ENUM(LoadingStatus)

  RiveQtQuickItem(QQuickItem *parent = nullptr);
  ~RiveQtQuickItem();

  Q_INVOKABLE void triggerAnimation(int id);

  std::unique_ptr<rive::ArtboardInstance> &artboardInstance() { return m_currentArtboardInstance; };

  bool isTextureProvider() const override { return true; }
  QSGTextureProvider *textureProvider() const override { return m_textureProvider.data(); }

  QString fileSource() const { return m_fileSource; }
  void setFileSource(const QString &source);

  LoadingStatus loadingStatus() const { return m_loadingStatus; }

  int currentArtboardIndex() const;
  void setCurrentArtboardIndex(int newCurrentArtboardIndex);

  const QVector<ArtBoardInfo> &artboards() const;
  const QVector<StateMachineInfo> &stateMachines() const { return m_stateMachineList; }
  const QVector<AnimationInfo> &animations() const;

  int currentStateMachineIndex() const;
  void setCurrentStateMachineIndex(int newCurrentStateMachineIndex);

  RiveQtStateMachineInputMap *stateMachineInterface() const;

  bool interactive() const;
  void setInteractive(bool newInteractive);

signals:
  void animationsChanged();
  void artboardsChanged();
  void stateMachinesChanged();

  void fileSourceChanged();
  void loadingStatusChanged();

  void currentArtboardIndexChanged();
  void currentStateMachineIndexChanged();

  void interactiveChanged();

  void internalArtboardChanged();
  void internalStateMachineChanged();
  void stateMachineInterfaceChanged();

protected:
  QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
#endif

  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

  void hoverMoveEvent(QHoverEvent *event) override;

private:
  void loadRiveFile(const QString &source);
  void updateAnimations();
  void updateStateMachines();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  friend RiveQSGRHIRenderNode;
  void renderOffscreen();
#endif

  bool hitTest(const QPointF &pos, const rive::ListenerType &type);

  QVector<ArtBoardInfo> m_artboardInfoList;
  QVector<AnimationInfo> m_animationList;
  QVector<StateMachineInfo> m_stateMachineList;

  std::unique_ptr<rive::File> m_riveFile;

  mutable QScopedPointer<QSGTextureProvider> m_textureProvider;

  QString m_fileSource;
  LoadingStatus m_loadingStatus { Idle };

  std::unique_ptr<rive::ArtboardInstance> m_currentArtboardInstance { nullptr };
  std::unique_ptr<rive::LinearAnimationInstance> m_animationInstance { nullptr };
  std::unique_ptr<rive::StateMachineInstance> m_currentStateMachineInstance { nullptr };

  bool m_scheduleArtboardChange { false };
  bool m_scheduleStateMachineChange { false };

  int m_currentArtboardIndex { -1 };
  int m_currentStateMachineIndex { -1 };
  int m_initialStateMachineIndex { -1 };

  RiveQtStateMachineInputMap *m_stateMachineInputMap { nullptr };

  RiveQtFactory m_riveQtFactory { RiveQtFactory::RiveQtRenderType::None };

  QElapsedTimer m_elapsedTimer;
  qint64 m_lastUpdateTime;
  bool m_geometryChanged = true;

  bool m_hasValidRenderNode = false;
  float m_lastMouseX { 0 };
  float m_lastMouseY { 0 };
  rive::ListenerType m_listenerType { rive::ListenerType::enter };

  RiveQSGRenderNode *m_renderNode { nullptr };
};
