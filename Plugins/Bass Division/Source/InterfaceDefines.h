/*
  ==============================================================================

    InterfaceDefines.h
    Created: 20 Feb 2021 9:08:40pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once

#define MAIN_PANEL_WIDTH            1050
#define MAIN_PANEL_HEIGHT           750

#define TOP_PANEL_WIDTH             MAIN_PANEL_WIDTH
#define TOP_PANEL_HEIGHT            30

#define ANALYSER_PANEL_WIDTH        MAIN_PANEL_WIDTH
#define ANALYSER_PANEL_HEIGHT       300

#define FFT_PANEL_WIDTH             ANALYSER_PANEL_WIDTH
#define FFT_PANEL_HEIGHT            ANALYSER_PANEL_HEIGHT

#define IO_PANEL_WIDTH              260
#define IO_PANEL_HEIGHT             ANALYSER_PANEL_HEIGHT

#define PROCESS_PANEL_WIDTH         MAIN_PANEL_WIDTH
#define PROCESS_PANEL_HEIGHT        320

#define XOVER_PANEL_WIDTH           40
#define XOVER_PANEL_HEIGHT          PROCESS_PANEL_HEIGHT

#define COMP_PANEL_WIDTH            ((MAIN_PANEL_WIDTH - (3 * XOVER_PANEL_WIDTH)) / 16 * 4.7
#define COMP_PANEL_HEIGHT           PROCESS_PANEL_HEIGHT

#define DIST_PANEL_WIDTH            ((MAIN_PANEL_WIDTH - (3 * XOVER_PANEL_WIDTH)) / 16 * 2.7
#define DIST_PANEL_HEIGHT           PROCESS_PANEL_HEIGHT

