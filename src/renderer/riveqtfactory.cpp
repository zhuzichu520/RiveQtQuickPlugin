
// SPDX-FileCopyrightText: 2023 Jeremias Bosch <jeremias.bosch@basyskom.com>
// SPDX-FileCopyrightText: 2023 basysKom GmbH
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "riveqtfactory.h"
#include "riveqtfont.h"
#include "riveqtpainterrenderer.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#  include "riveqtrhirenderer.h"
#else
#  include "riveqtopenglrenderer.h"
#endif

rive::rcp<rive::RenderBuffer> RiveQtFactory::makeBufferU16(rive::Span<const uint16_t> data)
{
  auto buffer = new RiveQtBufferU16(data.size());
  std::vector<uint16_t> vecData(data.begin(), data.end());
  buffer->setData(vecData);
  return rive::rcp<rive::RenderBuffer>(buffer);
}

rive::rcp<rive::RenderBuffer> RiveQtFactory::makeBufferU32(rive::Span<const uint32_t> data)
{
  auto buffer = new RiveQtBufferU32(data.size());
  std::vector<uint32_t> vecData(data.begin(), data.end());
  buffer->setData(vecData);
  return rive::rcp<rive::RenderBuffer>(buffer);
}

rive::rcp<rive::RenderBuffer> RiveQtFactory::makeBufferF32(rive::Span<const float> data)
{
  auto buffer = new RiveQtBufferF32(data.size());
  std::vector<float> vecData(data.begin(), data.end());
  buffer->setData(vecData);
  return rive::rcp<rive::RenderBuffer>(buffer);
}

rive::rcp<rive::RenderShader> RiveQtFactory::makeLinearGradient(float x1, float y1, float x2, float y2, const rive::ColorInt *colors,
                                                                const float *stops, size_t count)
{
  auto shader = new RiveQtLinearGradient(x1, y1, x2, y2, colors, stops, count);
  return rive::rcp<rive::RenderShader>(shader);
}

rive::rcp<rive::RenderShader> RiveQtFactory::makeRadialGradient(float centerX, float centerY, float radius, const rive::ColorInt colors[],
                                                                const float positions[], size_t count)
{
  auto shader = new RiveQtRadialGradient(centerX, centerY, radius, colors, positions, count);
  return rive::rcp<rive::RenderShader>(shader);
}

std::unique_ptr<rive::RenderPath> RiveQtFactory::makeRenderPath(rive::RawPath &rawPath, rive::FillRule fillRule)
{
  switch (m_renderType) {
  case RiveQtRenderType::QOpenGLRenderer:
    return std::make_unique<RiveQtOpenGLPath>(rawPath, fillRule);
  case RiveQtRenderType::QPainterRenderer:
    return std::make_unique<RiveQtPainterPath>(rawPath, fillRule);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  case RiveQtRenderType::RHIRenderer:
    return std::make_unique<RiveQtRhiGLPath>(rawPath, fillRule);
#endif
  case RiveQtRenderType::None:
    return std::make_unique<RiveQtPainterPath>(rawPath, fillRule); // TODO Add Empty Path
  }
}

std::unique_ptr<rive::RenderPath> RiveQtFactory::makeEmptyRenderPath()
{
  switch (m_renderType) {
  case RiveQtRenderType::QOpenGLRenderer:
    return std::make_unique<RiveQtOpenGLPath>();
  case RiveQtRenderType::QPainterRenderer:
    return std::make_unique<RiveQtPainterPath>();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  case RiveQtRenderType::RHIRenderer:
    return std::make_unique<RiveQtRhiGLPath>();
#endif
  case RiveQtRenderType::None:
    return std::make_unique<RiveQtPainterPath>(); // TODO Add Empty Path
  }
}

std::unique_ptr<rive::RenderPaint> RiveQtFactory::makeRenderPaint()
{
  return std::make_unique<RiveQtPaint>();
}

std::unique_ptr<rive::RenderImage> RiveQtFactory::decodeImage(rive::Span<const uint8_t> span)
{
  QByteArray imageData(reinterpret_cast<const char *>(span.data()), static_cast<int>(span.size()));
  QImage image = QImage::fromData(imageData);

  if (image.isNull()) {
    return nullptr;
  }
  return std::make_unique<RiveQtImage>(image);
}

rive::rcp<rive::Font> RiveQtFactory::decodeFont(rive::Span<const uint8_t> span)
{
  QByteArray fontData(reinterpret_cast<const char *>(span.data()), static_cast<int>(span.size()));
  int fontId = QFontDatabase::addApplicationFontFromData(fontData);

  if (fontId == -1) {
    return nullptr;
  }

  QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
  if (fontFamilies.isEmpty()) {
    return nullptr;
  }

  QFont font(fontFamilies.first());
  return rive::rcp<RiveQtFont>(new RiveQtFont(font, std::vector<rive::Font::Coord>()));
}
