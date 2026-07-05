Qt notices
==========

Thumbnail me 4 uses Qt 6 dynamically.

Components currently used:
- Qt6Core.dll
- Qt6Gui.dll
- Qt6Widgets.dll
- Qt platform plugins copied by windeployqt
- Qt image format plugins copied by the Windows ZIP release script
- Qt Image Formats WebP plugin: imageformats/qwebp.dll, when installed

Open-source licensing notes:
- Qt is available under commercial and open-source licenses.
- Qt 6 open-source use generally requires compliance with LGPL-3.0 or GPL
  terms, depending on the module.
- Qt Image Formats is available under commercial, LGPL-3.0, or GPL-2.0
  terms according to Qt documentation.
- Keep Qt DLLs and plugins dynamically linked and unmodified.
- Preserve the plugin folder layout used by Qt deployment:
    platforms/
    imageformats/
    styles/
    etc.

WebP:
- Qt Image Formats provides WebP read/write support through qwebp.dll.
- Qt documents the bundled libwebp third-party codec as BSD-3-Clause.

Release checklist:
- Include this notice.
- Include LGPL-3.0-only.txt.
- Include GPL-3.0-or-later.txt for GPL compatibility/reference.
- Include BSD-3-Clause.txt for libwebp attribution.
- Do not remove Qt copyright or license files if windeployqt copies them.
