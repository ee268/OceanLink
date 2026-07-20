#include "postlist.h"

#include <QVBoxLayout>

#include "ElaScrollBar.h"

PostList::PostList(QWidget *parent)
    : ElaScrollArea(parent)
{
    initContent();

    QStringList names = {"张三", "李四", "王五", "赵六", "钱七",
                         "孙八", "周九", "吴十", "郑十一", "王十二"};

    QStringList contents = {
        "今天天气真好，出去走走吧！",
        "在忙，稍后回复你。今天的工作特别多，需要处理很多事情，包括会议、报告和代码评审，希望能在下班前完成所有任务。下午还有两个会议要参加，一个是项目进度汇报，另一个是技术方案讨论，晚上可能还要加班处理一些紧急的bug修复工作。",
        "晚上一起吃饭吗？我听说附近新开了一家餐厅，菜品非常不错，环境也很好，很适合朋友聚会。他们家的招牌菜是红烧肉和清蒸鱼，还有很多特色小菜，价格也很实惠，我们约个时间一起去尝尝吧。",
        "明天开会记得带资料。",
        "周末去爬山吧，风景很美。我们可以早一点出发，带上水和食物，在山顶看日出，然后慢慢走下来，享受大自然的美好。上次去的时候拍了很多照片，这次我们可以带上相机，记录下更多美好的瞬间，晚上还可以在山脚下的民宿住一晚。",
        "刚到家，休息一下。",
        "学习中，勿扰。最近在学习Qt框架和C++编程，感觉还有很多需要掌握的知识点，每天都在进步。今天学习了信号槽机制和事件处理系统，做了几个小项目练手，明天准备学习模型视图编程和数据库操作，争取早日掌握Qt开发的核心技术。",
        "工作中，有事留言。",
        "旅行中，信号不好。这次去了一个很偏远的山区，风景非常壮观，但是手机信号很差，只能偶尔发一条消息。这里的空气非常清新，远离城市的喧嚣，让人心情特别舒畅，打算在这里多待几天，好好放松一下疲惫的身心。",
        "心情不错，分享一下。今天完成了一个很重要的项目，得到了领导的表扬，感觉所有的努力都值得了。这个项目从开始到现在经历了三个月的时间，期间遇到了很多困难和挑战，但是团队成员齐心协力，最终顺利完成了所有功能的开发和测试工作。"
    };

    for (int i = 0; i < 10; i++) {
        PostData data;
        data.name = names[i];
        data.account = QString("account_%1").arg(i + 1);
        data.date = "2026-07-20";
        data.content = contents[i];
        data.likeCnt = i * 3;
        data.commentCnt = i * 2;
        data.collectCnt = i;
        if (i % 2 == 0) {
            for (int j = 0; j < i / 2 + 1; j++) {
                data.imgList.append(QPixmap(":/resource/image/rupa.jpg"));
            }
        }
        addPost(data);
    }
}

void PostList::addPost(const PostData& data)
{
    PostItem* item = new PostItem(data, this->widget());
    _postList.append(item);

    connect(item, &PostItem::sigPostItemClicked, this, &PostList::sigPostItemClicked);
    this->widget()->layout()->addWidget(item);
}

void PostList::removePost(int index)
{
    if (index < 0 || index >= _postList.size())
        return;

    PostItem* item = _postList.takeAt(index);
    this->widget()->layout()->removeWidget(item);
    delete item;
}

void PostList::initContent()
{
    QWidget* contentWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(contentWidget);

    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(25);
    mainLayout->addStretch();

    contentWidget->setLayout(mainLayout);
    this->setWidget(contentWidget);
    this->setWidgetResizable(true);

    ElaScrollBar* scrollBar = new ElaScrollBar(this->verticalScrollBar(), this);
    scrollBar->setIsAnimation(true);

    contentWidget->setObjectName("PostContentWidget");
    contentWidget->setStyleSheet("#PostContentWidget { background-color: transparent; }");
}
