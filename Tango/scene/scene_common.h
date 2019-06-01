
#ifndef SCENE_COMMON_H
#define SCENE_COMMON_H

class MainWindow;


/* 场景 */
#include "Scene.h"

/* 标准库 */
#include <cctype>
#include <vector>
#include <functional>
#include <algorithm>
#include <random>
#include <ctime>

/* 工具库 */
#include <QStringList>
#include <QDebug>
#include <QLayout>

/* 控件库 */
#include <QPushButton>
#include <QRadioButton>
#include <QHeaderView>
#include <QScrollBar>
#include <QScrollArea>
#include <QTableWidget>
#include <QComboBox>

#include <QLabel>
#include <QLayout>
#include <QLineEdit>

/* 自定义类型 */
#include "../../TangoCommon/automator/GameAutomation.h"
#include "../../TangoCommon/automator/GameConfig.h"

#include "../../TangoCommon/client/Client.h"

#include "../../TangoCommon/types/RetriveMode.h"
#include "../../TangoCommon/types/UserStatus.h"
#include "../../TangoCommon/types/UserFullInfo.h"
#include "../../TangoCommon/types/UserBriefInfo.h"
#include "../../TangoCommon/types/TangoPair.h"

#include "../../TangoCommon/component/Logger.h"
#include "../../TangoCommon/component/MessageBox.h"
#include "../../TangoCommon/component/TimerWidget.h"
#include "../../TangoCommon/component/PairTableItem.h"
#include "../TangoCommon/component/ConfigSet.h"

const int DEFAULT_AUTHOR_PAGE_LIMIT = 10;
const int DEFAULT_CONSUMER_PAGE_LIMIT = 10;

#endif // SCENE_COMMON_H
