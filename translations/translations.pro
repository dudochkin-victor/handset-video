LANGUAGES =              # empty if only engineering English is needed!
CATALOGNAME = meegovideo # 
SOURCEDIR = $$PWD/..     # more then one directory is possible
TRANSLATIONDIR = $$PWD

include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_defines.prf)
include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_translations.prf)
