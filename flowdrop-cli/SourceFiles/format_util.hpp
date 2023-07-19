/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/libflowdrop/blob/master/LEGAL
 */
#pragma once

#include <string>

void printProgress(const std::string& fileName, std::size_t totalSize, std::size_t currentSize, bool last);
