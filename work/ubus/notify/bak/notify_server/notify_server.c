#include <unistd.h>
#include <libubox/blobmsg_json.h>
#include <libubox/uloop.h>
#include <libubus.h>
 
static struct ubus_context *ctx;
static pthread_t notify_thread_id = 0;
 
static void test_client_subscribe_cb(struct ubus_context *ctx, struct ubus_object *obj)
{
	fprintf(stderr, "Subscribers active: %d\n", obj->has_subscribers);
}


static int get_status(struct ubus_context *v_ctx, 
		struct ubus_object *v_obj, 
		struct ubus_request_data *v_req, 
		const char *v_method, 
		struct blob_attr *v_msg)
{
#if 0
	static struct blob_buf bbuf;
	blobmsg_buf_init(&bbuf);
	
	ubus_send_reply(v_ctx, v_req, bbuf.head);
#endif	
	printf("get status ...................\n");
	return 0;
}

/* 这个空的method列表，只是为了让object有个名字，这样client可以通过object name来订阅。 */
static struct ubus_method test_methods[] = {
	UBUS_METHOD_NOARG("get_status", get_status),
};
 
static struct ubus_object_type test_obj_type = 
	UBUS_OBJECT_TYPE("etm_test", test_methods);
 
static struct ubus_object test_object = {
	.name = "etm_test", /* object的名字 */
	.type = &test_obj_type,
	.methods = test_methods,
	.n_methods = ARRAY_SIZE(test_methods),
	.subscribe_cb = test_client_subscribe_cb,
};

void notify_thread_main()
{
	while (1) {
		printf("while (1): xxxxxxxxxxxxxxxxxxxx  0\n");
		sleep(2);
		/* step2: 广播notification消息。 */
		static struct blob_buf b;
		blob_buf_init(&b, 0);
		 
		/* 需要传递的参数 */
		blobmsg_add_string(&b, "path", "/media/sda/a.out");
		blobmsg_add_string(&b, "cid", "asdfasdfasdf");
		blobmsg_add_string(&b, "gcid", "asdfq4tiuqseyri");

		ubus_notify(ctx,  &test_object, "etm_test", b.head, -1);
	}
}
 
static void notifier_main(void)
{
	int ret = 0;
 
	/* 注册一个object，client可以订阅这个object */
	printf("add object: begin\n");
	ret = ubus_add_object(ctx, &test_object);
	if (ret) {
		fprintf(stderr, "Failed to add object: %s\n", ubus_strerror(ret));
		return;
	}
	printf("add object: end ret=%s\n", ubus_strerror(ret));
 
	/* 在需要的时候，向所有客户端发送notify消息 */
	
	/* step1: 如果需要传递参数，则保存到struct blob_attr类型的结构体中。 */
 
	/* 
	int ubus_notify(struct ubus_context *ctx, struct ubus_object *obj, const char *type, struct blob_attr *msg, int timeout);
	type是一个字符串，自定义的。msg是需要携带的参数。如果需要等待回复，timeout需设置为>=0。
	*/
	ret = pthread_create(&notify_thread_id, NULL, notify_thread_main, NULL);
	if (ret != 0)
        {
	        printf("create ubus thread fail:%d\n", ret);
	}
}
 
int main(int argc, char **argv)
{
	const char *ubus_socket = "/var/run/ubus.sock";
 
	uloop_init();
 
	ctx = ubus_connect(ubus_socket);
	if (!ctx) {
		fprintf(stderr, "Failed to connect to ubus\n");
		return -1;
	}
 
	ubus_add_uloop(ctx);
 
	printf("xxxxxxxxxxxxxxxx     0 \n");
	notifier_main();
	
	printf("xxxxxxxxxxxxxxxx     1 \n");
	uloop_run();
 
	printf("xxxxxxxxxxxxxxxx     2 \n");
	ubus_free(ctx);
	uloop_done();
 
	return 0;
}
