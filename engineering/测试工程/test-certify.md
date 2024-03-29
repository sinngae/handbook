# 测试分级认证
[参考](https://mike-bland.com/2011/10/18/test-certified.html)

常常会遇到此类问题：
+ 代码难以测试
+ 不知道怎么写测试
+ 不知道怎么设计 `测试的能力`
+ 从未见过良好测试的项目是怎么做的
+ 从未经历死亡行军（）
+ 测试活动不是我的权责，所以不做
+ 没有时间测试
+ 永远都在构建代码
+ 存量用例测试运行太慢
+ 一直都处于交付功能的压力下
+ 几乎没有可以学习的测试活动
+ 工具太慢，没法用于测试

2006年，Google内部Testing Grouplet(20%时间项目，Gmail也是)旨在推动开发人员测试，转变开发不测试的文化。
+ 提高对自动化测试重要性的认识
+ 通过不断改进测试框架和组件来减轻编写测试的痛苦
+ 提供信息和指导开发人员达到良好的测试实践
+ Debugging sucks，Testing rocks

Testing Grouplet下面又包含几个项目，如Testing on the Toilet(每周在厕所里贴张测试小技巧)， Design for testability principles(代码可测试性)还有今天要介绍的Test Certified。
以上交代下TC背景，下面开始解说下TC Level 1，如何从0到1。
## Test Level 1
完成Level 1大概也就1个月的时间，这个过程可以让测试人员和开发人员紧密的合作起来，同时也给开发人员不断灌输质量的意识。
### Set up test coverage bundles
代码覆盖率系统可以识别你运行的测试针对的是哪行代码，这有2个好处，1个可以查看哪些地方需要更好的覆盖，另一个是可以衡量覆盖率，以便改进。内部基础工具提供了一个代码覆盖工具，只需要在build文件中加上配置，那么在代码审查的时候，代码审查系统会自动计算覆盖率，默认使用的是语句覆盖，如果需要条件/判定覆盖，那么修改配置就好。
Google内部是使用blaze(现在开源了，叫Bazel)来编译代码，只需要配置build文件，对于开发人员来说没有压力，第一步就这样走出了。
### Set up a continuous build
搭建持续集成，相信很多公司已经这么做了。Google的持续集成系统叫做TAP (Test Automation Platform)，开发每提交一个CL(change list)， TAP系统就会进行编译和测试。
但是仅仅是持续集成还是不够，如果编译失败或是里面的测试失败，需要及时的修复，所以需要从开发人员中征集志愿者来组成一个build cop，及时的处理失败，这样后续的发布系统才能拿到跑绿的Cut CL。
### Classify your tests as Small, Medium, and Large
将测试分成小型、中型、大型，为什么不叫单元测试、集成测试、系统测试呢，这是因为要和系统资源匹配，当在代码中指定测试的size(一个简单的标识)，那么基础工具分配给相应的资源。虽说Google的服务器分布在世界各地，几百万上千万的服务器分配到内部成千上万的项目后也是紧巴巴的。
理想状态下你需要很多小规模快速的测试来覆盖大部分的代码，这样你能够快速的得到运行结果来修复问题。这是一个单元测试养成习惯的过程，随着项目的进展，提交单元测试成为吃饭睡觉一样的自然。
### Identify nondeterministic tests
识别不确定行测试。对同一个代码运行测试后会得到不一致的行为，可能有几个原因，比如测试依赖外部系统，测试运行需要某种特定条件(时间或地点)，相互干扰的测试。
那么你肯定不希望这类测试打断持续集成，这样会浪费时间精力来定位到底是失败还是flaky，提前隔离出来单独运行分析。同样的在build文件中可以加入规则。
### Create a smoke test suite
创建一个冒烟测试集，冒烟测试不能发现所有问题，但是可以保证产品功能主干正常运行，可以发现最大的问题。持续集成加入冒烟测试后按每CL的频率运行，这样团队对产品的信心更强。

## Test Level 2
Level2除了持续发布的配置，后几项都与测试代码相关，从中可以看出，想要获得认证，需要搭建起UT框架，填充测试用例；需要搭建UI测试框架，并包含一个用例验证一个功能点
### No releases with red tests
基于Level1搭建的持续集成，持续发布选用的CL(change list)就可以取自CI系统最后跑通的CL，因为持续集成中包含了冒烟测试，那么发布到开发或测试环境上的系统，测试关注的是其他类别的用例。

### Require smoke test suite to pass before a submit
在Level1中已经划分出了冒烟测试集，在每次代码审核通过后，开发在提交代码前再次运行冒烟测试，这样保证每次提交代码，冒烟测试都是通过的，节省了后期修复的成本。(不知道你是否遇到过开发提交代码后通知测试可以测了，可是你一访问系统就碰到问题，所以类似的BVT是强制的，并且推到每一次代码提交)
代码审核工具可以定义规则，只有通过冒烟测试后才能成功提交代码到代码库，如果失败，发送邮件给开发。
### Incremental coverage by all tests >= 50%
这一步是真正的需要提交测试代码。你团队的新代码至少要有50%是有测试代码的。它没有规定一定是哪种测试类型，也没有规定哪部分代码，也不是每次变更都要有50%覆盖率，而是新代码总的覆盖率要有50%。这样方便团队灵活的决定在何处何时提高测试覆盖率。开发可以通过TAP工具跟踪增加的覆盖率。
### Incremental coverage by small tests >= 10%
新代码小型测试覆盖率至少10%。从历史数据上看，小型/中型/大型的比例是70/20/10，这个比率不是硬性规定，可以给各团队作为参考。

### At least one feature tested by an integration test
虽然小型测试很重要，用来验证各个独立组件是否工作正常，但同时也需要更大的端到端的测试来验证系统功能是否正常工作。这个指标要求至少有一个集成测试来验证一个功能点。比如一个webdriver用例来验证web应用。

## Test Level 3
### Require tests for all nontrivial changes
所有变更都需要有对应的测试，这个要求开始需要辛勤的劳动了。换句话说，这也是项目受益的开始。没有测试你无法发现开发过程中的问题，对你的代码修改是否工作没有信心。测试是对代码健康和稳定的投资，部分经验和数据表明，Google认为测试是一个软件项目专业与否的标志。所以这项指标是团队承诺将测试当作开发流程一部分的关键点。

### Incremental coverage by small tests >= 50%
小型测试覆盖率至少50%，这是Level2的延续。

### New significant features are tested by integration tests
新做的大功能都要有对应的集成测试，这个重要功能根据团队自己定义。