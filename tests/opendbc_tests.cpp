#include <fstream>
#include <gtest/gtest.h>

#include "dbcparser.h"
#include "log.hpp"
#include "opendbc_tests_expected_data.hpp"
#include "test_helper.hpp"

#include <filesystem>

struct OpenDBCTest : public ::testing::TestWithParam<std::string> {
    CANdb::DBCParser parser;
};

TEST_P(OpenDBCTest, parse_dbc_file)
{
    auto dbc_file = GetParam();
    auto file = test_helper::loadDBCFile("opendbc/" + dbc_file);
    const auto db = parser.parse(file);
    ASSERT_TRUE(db);

    if (dbc_file == "tesla_can.dbc") {
        EXPECT_EQ(db->symbols, test_data::dbc_data.at(dbc_file).symbols);
        EXPECT_EQ(db->ecus, test_data::dbc_data.at(dbc_file).bu);
    }
}

TEST(OpenDBCTest, all_tests)
{
    GTEST_SKIP();
    const auto path = fs::path{ fs::path{ DBC_DIR } / fs::path{ "opendbc" } };

    for (const auto& p : std::filesystem::directory_iterator(path)) {
        CANdb::DBCParser parser;
        if (p.is_regular_file()) {
            const auto filepath = "opendbc/" + p.path().filename().string();
            auto file = test_helper::loadDBCFile(filepath);
            const auto db = parser.parse(file);
            if (db) {
                std::cout << "Success " << filepath << "\n";
            } else {
                std::cout << "Failure " << filepath << "\n";
            }
        }
    }
}

INSTANTIATE_TEST_SUITE_P(OpenDBC, OpenDBCTest,
    ::testing::Values("acura_ilx_2016_nidec.dbc", "gm_global_a_chassis.dbc", "gm_global_a_lowspeed.dbc",
        "gm_global_a_powertrain.dbc", "honda_accord_touring_2016_can.dbc", "hyundai_kia_generic.dbc",
        "cadillac_ct6_powertrain.dbc", "volvo_v40_2017_pt.dbc", "hyundai_2015_ccan.dbc", "gm_global_a_powertrain.dbc",
        "nissan_leaf_2018.dbc", "toyota_tss2_adas.dbc", "hyundai_2015_mcan.dbc", "ford_fusion_2018_adas.dbc",
        "cadillac_ct6_chassis.dbc", "ESR.dbc", "chrysler_pacifica_2017_hybrid.dbc", "acura_ilx_2016_nidec.dbc",
        "gm_global_a_lowspeed.dbc", "toyota_prius_2010_pt.dbc", "gm_global_a_chassis.dbc",
        "mercedes_benz_e350_2010.dbc", "chrysler_pacifica_2017_hybrid_private_fusion.dbc",
        "ford_cgea1_2_bodycan_2011.dbc", "toyota_adas.dbc", "luxgen_s5_2015.dbc", "mazda_2017.dbc",
        "toyota_2017_ref_pt.dbc", "vw_golf_mk4.dbc", "nissan_x_trail_2017.dbc", "volvo_v60_2015_pt.dbc",
        "honda_accord_touring_2016_can.dbc", "gm_global_a_lowspeed_1818125.dbc", "mazda_3_2019.dbc", "bmw_e9x_e8x.dbc",
        "cadillac_ct6_object.dbc",
        "mercedes_benz_e350_2010.dbc",
        "toyota_prius_2010_pt.dbc",
        "tesla_can.dbc",
        "tesla_radar.dbc",
//        "FORD_CADS.dbc",
//        "acura_ilx_2016_can_generated.dbc",
//        "acura_rdx_2020_can_generated.dbc",
        "ford_cgea1_2_ptcan_2011.dbc",
//        "ford_fusion_2018_pt.dbc",
//        "ford_lincoln_base_pt.dbc",
//        "gm_global_a_high_voltage_management.dbc",
//        "gm_global_a_object.dbc",
//        "gm_global_a_powertrain_expansion.dbc",
//        "honda_accord_lx15t_2018_can_generated.dbc",
//        "honda_accord_s2t_2018_can_generated.dbc",
//        "honda_civic_hatchback_ex_2017_can_generated.dbc",
//        "honda_civic_sedan_16_diesel_2019_can_generated.dbc",
//        "honda_civic_touring_2016_can_generated.dbc",
//        "honda_clarity_hybrid_2018_can_generated.dbc",
//        "honda_crv_ex_2017_body_generated.dbc",
//        "honda_crv_ex_2017_can_generated.dbc",
//        "honda_crv_executive_2016_can_generated.dbc",
//        "honda_crv_hybrid_2019_can_generated.dbc",
//        "honda_crv_touring_2016_can_generated.dbc",
//        "honda_fit_ex_2018_can_generated.dbc",
//        "honda_fit_hybrid_2018_can_generated.dbc",
//        "honda_hrv_touring_2019_can_generated.dbc",
//        "honda_insight_ex_2019_can_generated.dbc",
//        "honda_odyssey_exl_2018_generated.dbc",
//        "honda_odyssey_extreme_edition_2018_china_can_generated.dbc",
//        "honda_pilot_touring_2017_can_generated.dbc",
//        "honda_ridgeline_black_edition_2017_can_generated.dbc",
        "hyundai_i30_2014.dbc",
//        "lexus_ct200h_2018_pt_generated.dbc",
//        "lexus_gs300h_2017_pt_generated.dbc",
//        "lexus_is_2018_pt_generated.dbc",
//        "lexus_nx300_2018_pt_generated.dbc",
//        "lexus_nx300h_2018_pt_generated.dbc",
//        "lexus_rx_350_2016_pt_generated.dbc",
//        "lexus_rx_hybrid_2017_pt_generated.dbc",
//        "subaru_forester_2017_generated.dbc",
//        "subaru_global_2017_generated.dbc",
//        "subaru_global_2020_hybrid_generated.dbc",
//        "subaru_outback_2015_generated.dbc",
//        "subaru_outback_2019_generated.dbc",
//        "toyota_avalon_2017_pt_generated.dbc",
//        "toyota_camry_hybrid_2018_pt_generated.dbc",
//        "toyota_corolla_2017_pt_generated.dbc",
//        "toyota_highlander_2017_pt_generated.dbc",
//        "toyota_highlander_hybrid_2018_pt_generated.dbc",
        "toyota_iQ_2009_can.dbc",
//        "toyota_nodsu_hybrid_pt_generated.dbc",
//        "toyota_nodsu_pt_generated.dbc",
//        "toyota_prius_2017_pt_generated.dbc",
//        "toyota_rav4_2017_pt_generated.dbc",
//        "toyota_rav4_hybrid_2017_pt_generated.dbc",
//        "toyota_sienna_xle_2018_pt_generated.dbc",
        "vw_mqb_2010.dbc"));
