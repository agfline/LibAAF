#include <check.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "include/libaaf/LibCFB.h"

/* Invariant: cfb_open must not crash or read out-of-bounds regardless of
   _uSectorShift value embedded in a crafted CFB file header. */

static const char *build_cfb_file(uint16_t sector_shift, const char *path) {
    /* Minimal CFB header: 512 bytes, magic + sector shift at offset 30 */
    uint8_t buf[512];
    memset(buf, 0, sizeof(buf));
    /* CFB magic */
    const uint8_t magic[8] = {0xD0,0xCF,0x11,0xE0,0xA1,0xB1,0x1A,0xE1};
    memcpy(buf, magic, 8);
    /* Minor version = 0x003E at offset 24 */
    buf[24] = 0x3E; buf[25] = 0x00;
    /* Major version = 3 at offset 26 */
    buf[26] = 0x03; buf[27] = 0x00;
    /* Byte order = little-endian at offset 28 */
    buf[28] = 0xFE; buf[29] = 0xFF;
    /* _uSectorShift at offset 30 */
    buf[30] = (uint8_t)(sector_shift & 0xFF);
    buf[31] = (uint8_t)((sector_shift >> 8) & 0xFF);
    /* _uMiniSectorShift at offset 32 */
    buf[32] = 0x06; buf[33] = 0x00;
    /* Number of FAT sectors = 0 at offset 44 */
    /* DIFAT first = FREESECT at offset 68 */
    buf[68]=0xFE; buf[69]=0xFF; buf[70]=0xFF; buf[71]=0xFF;

    FILE *f = fopen(path, "wb");
    if (!f) return NULL;
    fwrite(buf, 1, sizeof(buf), f);
    fclose(f);
    return path;
}

START_TEST(test_sector_shift_oob)
{
    /* Payloads: exploit (31), boundary (16), valid (9) */
    uint16_t shifts[] = { 31, 16, 9 };
    const char *tmpfile = "/tmp/test_cfb_sector_shift.cfb";

    for (int i = 0; i < 3; i++) {
        if (!build_cfb_file(shifts[i], tmpfile)) continue;

        /* The invariant: cfb_open must not cause undefined behaviour.
           We treat any return (NULL or valid pointer) as acceptable;
           what must NOT happen is a crash/OOB. */
        cfb_s *cfbd = cfb_open(tmpfile, NULL, 0, NULL);
        /* If it opened, close cleanly */
        if (cfbd != NULL) {
            cfb_close(cfbd);
        }
        /* Reaching here means no crash — invariant holds */
        ck_assert_msg(1, "cfb_open must not crash on shift=%u", shifts[i]);
    }
    remove(tmpfile);
}
END_TEST

Suite *security_suite(void) {
    Suite *s = suite_create("Security");
    TCase *tc = tcase_create("SectorShiftOOB");
    tcase_add_test(tc, test_sector_shift_oob);
    suite_add_tcase(s, tc);
    return s;
}

int main(void) {
    int failed;
    Suite *s = security_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}