
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

#include <QLabel>
#include <QLayout>
#include <QLineEdit>

/* 自定义类型 */
#include "../../TangoCommon/client/GameAutomation.h"
#include "../../TangoCommon/client/GameConfig.h"
#include "../../TangoCommon/client/Client.h"

#include "../../TangoCommon/types/MessageBox.h"
#include "../../TangoCommon/types/TimerWidget.h"

#include "../../TangoCommon/types/PairTableItem.h"
#include "../../TangoCommon/types/RetriveMode.h"
#include "../../TangoCommon/types/UserStatus.h"
#include "../../TangoCommon/types/UserFullInfo.h"
#include "../../TangoCommon/types/UserBriefInfo.h"
#include "../../TangoCommon/types/TangoPair.h"


const int DEFAULT_CREATION_TABLE_ITEMS_COUNT = 3;
const int DEFAULT_AUTHOR_PAGE_LIMIT = 10;
const int DEFAULT_CONSUMER_PAGE_LIMIT = 10;

#endif // SCENE_COMMON_H
