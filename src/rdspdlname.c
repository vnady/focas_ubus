
enum {
    RDSPDLNAME_HANDLE,
    RDSPDLNAME_M,
    __RDSPDLNAME_MAX
};

static const struct blobmsg_policy policy_rdspdlname[__RDSPDLNAME_MAX] = {
    [RDSPDLNAME_HANDLE] = { .name = "handle", .type = BLOBMSG_TYPE_INT32 },
    [RDSPDLNAME_M] = { .name = "m", .type = BLOBMSG_TYPE_INT32 },
};

static int focas_rdspdlname(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
	struct blob_attr *tb[__RDSPDLNAME_MAX];
	unsigned short handle = 0;
	int m = 0;
	short ret = 0;
	int i = 0;
	void *cookie = NULL;
	ODBSPDLNAME data[MAX_AXIS];
	short number = MAX_AXIS;
	memset(data, 0, sizeof(ODBSPDLNAME) * MAX_AXIS);

	blobmsg_parse(policy_rdspdlname, __RDSPDLNAME_MAX, tb, blob_data(msg), blob_len(msg));
	if (!tb[RDSPDLNAME_HANDLE])
		return UBUS_STATUS_INVALID_ARGUMENT;

	handle = blobmsg_get_u32(tb[RDSPDLNAME_HANDLE]);
	if (tb[RDSPDLNAME_M])
		m = blobmsg_get_u32(tb[RDSPDLNAME_M]);

	/*
	if (m != 0)
		ret = cnc_rdspdlnamem(handle, &number, data);
	else
		ret = cnc_rdspdlname(handle, &number, data);
		*/
	ret = cnc_rdspdlname(handle, &number, data);

	CHECK_FOCAS_RET(ret);
	blob_buf_init(&b, 0);
	blobmsg_add_u32(&b, "rc", ret);

	cookie = blobmsg_open_array(&b, "data");
	for (i = 0; i < number; ++i) {
		char buf[sizeof(ODBSPDLNAME) + 1];
		memset(buf, 0, sizeof(ODBSPDLNAME) + 1);
		memcpy(buf, &data[i], sizeof(ODBSPDLNAME));
		blobmsg_add_string(&b, NULL, buf);
	}
	blobmsg_close_array(&b, cookie);

	ubus_send_reply(ctx, req, b.head);

	return 0;
}

