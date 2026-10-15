## gin内置中间件
func BasicAuth(accounts Accounts) HandlerFunc
func BasicAuthForRealm(accounts Accounts, realm string) HandlerFunc
func Bind(val interface{}) HandlerFunc //拦截请求参数并进行绑定
func ErrorLogger() HandlerFunc       //错误日志处理
func ErrorLoggerT(typ ErrorType) HandlerFunc //自定义类型的错误日志处理
func Logger() HandlerFunc //日志记录
func LoggerWithConfig(conf LoggerConfig) HandlerFunc
func LoggerWithFormatter(f LogFormatter) HandlerFunc
func LoggerWithWriter(out io.Writer, notlogged ...string) HandlerFunc
func Recovery() HandlerFunc
func RecoveryWithWriter(out io.Writer) HandlerFunc
func WrapF(f http.HandlerFunc) HandlerFunc //将http.HandlerFunc包装成中间件
func WrapH(h http.Handler) HandlerFunc //将http.Handler包装成中间件

## 中间件数据传递
func (c *Context) Set(key string, value interface{})

func (c *Context) Get(key string) (value interface{}, exists bool)

func (c *Context) GetBool(key string) (b bool)
func (c *Context) GetDuration(key string) (d time.Duration)
func (c *Context) GetFloat64(key string) (f64 float64)
func (c *Context) GetInt(key string) (i int)
func (c *Context) GetInt64(key string) (i64 int64)
func (c *Context) GetString(key string) (s string)
func (c *Context) GetStringMap(key string) (sm map[string]interface{})
func (c *Context) GetStringMapString(key string) (sms map[string]string)
func (c *Context) GetStringMapStringSlice(key string) (smss map[string][]string)
func (c *Context) GetStringSlice(key string) (ss []string)
func (c *Context) GetTime(key string) (t time.Time)

## 中间件拦截
func (c *Context) Abort()
func (c *Context) AbortWithError(code int, err error) *Error
func (c *Context) AbortWithStatus(code int)

func (c *Context) AbortWithStatusJSON(code int, jsonObj interface{})

## 中间件前置后置
func MyMiddleware(c *gin.Context){
    //请求前
    c.Next()
    //请求后
}
