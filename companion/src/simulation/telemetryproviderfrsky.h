/*
 * Copyright (C) EdgeTX
 *
 * Based on code named
 *   opentx - https://github.com/opentx/opentx
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#pragma once

#include <QWidget>
#include <QDateTime>

#include "simulatorinterface.h"
#include "telemetryprovider.h"

namespace Ui {
  class TelemetryProviderFrSky;
}

class TelemetryProviderFrSky : public QWidget, public TelemetryProvider
{
  Q_OBJECT

  public:

    explicit TelemetryProviderFrSky(QWidget * parent);
    virtual ~TelemetryProviderFrSky();

  signals:
    void telemetryDataChanged(const quint8 protocol, const QByteArray data);

  public slots:
    bool generateSportPacket(uint8_t * packet, uint8_t dataId, uint8_t prim, uint16_t appId, uint32_t data);
    void resetRssi();
    QHash<QString, QString> * getSupportedLogItems();
    void loadItemFromLog(QString itemName, QString value);
    QString getLogfileIdentifier();
    void loadUiFromSimulator(SimulatorInterface * simulator);

  protected slots:
    void updateGps();
    void generateTelemetryFrame(SimulatorInterface * simulator);
    void refreshSensorRatios(SimulatorInterface * simulator);

  protected:
    Ui::TelemetryProviderFrSky * ui;
    QTimer gpsTimer;
    QHash<QString, QString> supportedLogItems;

    class FlvssEmulator
    {
      public:
        static const uint32_t MAXCELLS = 8;
        uint32_t setAllCells_GetNextPair(double cellValues[MAXCELLS]);

      private:
        void encodeAllCells();
        void splitIntoCells(double totalVolts);
        static uint32_t encodeCellPair(uint8_t cellNum, uint8_t firstCellNo, double cell1, double cell2);
        double cellFloats[MAXCELLS];
        uint32_t nextCellNum;
        uint32_t numCells;
        uint32_t cellData1;
        uint32_t cellData2;
        uint32_t cellData3;
        uint32_t cellData4;
    };  // FlvssEmulator

    class GPSEmulator
    {
      public:
        GPSEmulator();
        uint32_t getNextPacketData(uint32_t packetType);
        void setGPSDateTime(QString dateTime);
        void setGPSLatLon(QString latLon);
        void setGPSCourse(double course);
        void setGPSSpeedKMH(double speed);
        void setGPSAltitude(double altitude);

      private:
        QDateTime dt;
        bool sendLat;
        bool sendDate;
        double lat;
        double lon;
        double course;
        double speedKNTS;
        double altitude; // in meters
        uint32_t encodeLatLon(double latLon, bool isLat);
        uint32_t encodeDateTime(uint8_t yearOrHour, uint8_t monthOrMinute, uint8_t dayOrSecond, bool isDate);
    };  // GPSEmulator

  private slots:
    void on_saveTelemetryvalues_clicked();
    void on_loadTelemetryvalues_clicked();
    void on_GPSpushButton_clicked();
    void on_gps_course_valueChanged(double arg1);
};
