#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <hardware/audio_effect.h>

static effect_descriptor_t test_descriptor =
{
    { 0x2880c296, 0xd02e, 0x11e9, 0xbb65, { 0x2a, 0x2a, 0xe2, 0xdb, 0xcc, 0xe4 } },
    { 0x2880c53e, 0xd02e, 0x11e9, 0xbb65, { 0x2a, 0x2a, 0xe2, 0xdb, 0xcc, 0xe4 } }, // own UUID
    EFFECT_CONTROL_API_VERSION,
    EFFECT_FLAG_TYPE_INSERT | EFFECT_FLAG_INSERT_FIRST,
    10,
    1,
    "Test main unit",
    "Test"
};

/* Library mandatory methods. */
struct effect_module_s
{
    const struct effect_interface_s *itfe;
    void *data; // TODO: allocate needed data
    effect_descriptor_t *descriptor;
};
static int32_t generic_process(effect_handle_t self, audio_buffer_t *in, audio_buffer_t *out)
{
    struct effect_module_s *e = (struct effect_module_s *) self;
    // TODO: process buffer here
    return 0;
}
static int32_t generic_command(effect_handle_t self, uint32_t cmdCode, uint32_t cmdSize, void *pCmdData, uint32_t *replySize, void *pReplyData)
{
    struct effect_module_s *e = (struct effect_module_s *) self;
    // TODO: process command here
    return 0;
}
static int32_t generic_getDescriptor(effect_handle_t self, effect_descriptor_t *pDescriptor)
{
    struct effect_module_s *e = (struct effect_module_s *) self;
    memcpy(pDescriptor, e->descriptor, sizeof(effect_descriptor_t));
    return 0;
}
static const struct effect_interface_s generic_interface =
{
    generic_process,
    generic_command,
    generic_getDescriptor,
    NULL
};
int32_t effectCreate(const effect_uuid_t *uuid, int32_t sessionId, int32_t ioId, effect_handle_t *pEffect)
{
    int f = open("/log.txt", O_WRONLY | O_APPEND | O_CREAT);
    if (f != -1)
    {
        write(f, "ec\n", 3);
        close(f);
    }

    struct effect_module_s *e = (struct effect_module_s *) calloc(1, sizeof(struct effect_module_s));
    e->itfe = &generic_interface;
    e->data = malloc(10); // TODO: allocate data here
    e->descriptor = &test_descriptor;
    *pEffect = (effect_handle_t) e;
    return 0;
}
int32_t effectRelease(effect_handle_t ei)
{
    struct effect_module_s *e = (struct effect_module_s *) ei;
    free(e->data); // TODO: delete data here
    free(e);
    return 0;
}
int32_t effectGetDescriptor(const effect_uuid_t *uuid, effect_descriptor_t *pDescriptor)
{
    size_t descriptorSize = sizeof(effect_descriptor_t);
    memcpy(pDescriptor, &test_descriptor, descriptorSize);
    return 0;
}
audio_effect_library_t AUDIO_EFFECT_LIBRARY_INFO_SYM =
{
    .tag = AUDIO_EFFECT_LIBRARY_TAG,
    .version = EFFECT_LIBRARY_API_VERSION,
    .name = "Test Effect Library",
    .implementor = "Test",
    .create_effect = effectCreate,
    .release_effect = effectRelease,
    .get_descriptor = effectGetDescriptor,
};
