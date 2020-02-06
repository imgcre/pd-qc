#pragma once
#include "main.h"
#include "i2c_device.hpp"
#include "singleton.hpp"
#include "exti.hpp"

namespace Bsp {
  using namespace Hal;
  using namespace Utilities;
  class Sc8812a : public Singleton<Sc8812a> {
    friend class Singleton<Sc8812a>;
    Sc8812a(I2C_HandleTypeDef* hi2c) : i2c(hi2c), vBus(this), vBat(this), iBus(this), iBat(this) {
      Exti::reg(SC8812A_INT_Pin, [this]{intCallback();});
    }
  public:

    void init() {
      //csel = 4S���
      if (true /*TODO: STM32ĳ����*/) {
        //vcell = 4.2V
        i2c[Reg::VBAT_SET] = 0b00011001;
      } else {
        //vcell = 4.35V
        i2c[Reg::VBAT_SET] = 0b00011100;
      }
  
      //ibat_r = 6x
      //ibus_r = 6x
      i2c[Reg::RATIO] = 0b00100100;
  
      //dt_set = 60ns
      i2c[Reg::CTRL0_SET] = 0b00000110;
  
  
      //pmos����(���ܻ��õ�) [bit7]
      //����adc
      i2c[Reg::CTRL3_SET] = 0b00100010;
  
      //sc8812a[0x0c] vbus����ֵ RO
      //sc8812a[0x0d] vbus����ֵ2 RO
      //vbusֵ = (4 * vbus����ֵ + vbus����ֵ2 + 1) * vbus_r * 2mV
      //vbat��ibus��ibatֵ����
  
      //sc8812a[0x17] ״̬�Ĵ��� RO
      //sc8812a[0x19] �ж�MASK�Ĵ��� RW
  
      //����
      i2c[Reg::CTRL2_SET] = 0b00001001;
    }
    
  private:
    //�κ�Status�Ĵ����е�λ����λ�����´����ж�
    //�ж��źŸ�����=0.85ms
    //NOTE: ���ڴ����߳�ģʽ
    void intCallback() {
      auto status = *i2c[Reg::STATUS];
      if (status & Status::AC_OK) {
        //����������
        //����Ϊ������ģʽ
        setCharingMode();
        setIBusLim(0.1); //100mA
        osDelay(100);
        //ip2721
        
        //���ó�����Ϊ100mA
        
      }
    }
    
  public:
    
    enum class Reg { 
      VBAT_SET,
      VBUSREF_I_SET,
      VBUSREF_I_SET_2,
      VBUSREF_E_SET,
      VBUSREF_E_SET_2,
      IBUS_LIM_SET,
      IBAT_LIM_SET,
      VINREG_SET,
      RATIO,
      CTRL0_SET,
      CTRL1_SET,
      CTRL2_SET,
      CTRL3_SET,
      VBUS_FB_VALUE,
      VBUS_FB_VALUE_2,
      VBAT_FB_VALUE,
      VBAT_FB_VALUE_2,
      IBUS_VALUE,
      IBUS_VALUE_2,
      IBAT_VALUE,
      IBAT_VALUE_2,
      STATUS = 0x17,
      MASK = 0x19,
      DP_DM_CTRL,
      DP_DM_READ,
    };
    
    enum Status : uint8_t { 
      DM_L = 1 << 7,
      AC_OK = 1 << 6,
      INDET2 = 1 << 5,
      INDET1 = 1 << 4,
      VBUS_SHORT = 1 << 3,
      OTP = 1 << 2,
      EOC = 1 << 1,
      Reserved = 1 << 0,
    };
    
    class VBus {
    public:
      VBus(Sc8812a* sc8812a) : sc8812a(sc8812a) { }
      operator float() {
        auto vbusValue = sc8812a->i2c[Reg::VBUS_FB_VALUE];
        auto vbusValue2 = sc8812a->i2c[Reg::VBUS_FB_VALUE_2] >> 6;
        return (vbusValue * 4 + vbusValue2 + 1) * 12.5 * 2 / 1000;
      }
    private:
      Sc8812a* sc8812a;
    } vBus; //��λΪV
    
    class VBat {
    public:
      VBat(Sc8812a* sc8812a) : sc8812a(sc8812a) { }
      operator float() {
        auto vbatValue = sc8812a->i2c[Reg::VBAT_FB_VALUE];
        auto vbatValue2 = sc8812a->i2c[Reg::VBAT_FB_VALUE_2] >> 6;
        return (vbatValue * 4 + vbatValue2 + 1) * 12.5 * 2 / 1000;
      }
    private:
      Sc8812a* sc8812a;
    } vBat; //��λΪV
    
    class IBus {
    public:
      IBus(Sc8812a* sc8812a) : sc8812a(sc8812a) { }
      operator float() {
        auto ibusValue = sc8812a->i2c[Reg::IBUS_VALUE];
        auto ibusValue2 = sc8812a->i2c[Reg::IBUS_VALUE_2] >> 6;
        return (4 * ibusValue + ibusValue2 + 1) * 2 / 1200. * 6 * 10 / sc8812a->kRs1;
      }
    private:
      Sc8812a* sc8812a;
    } iBus; //��λΪA
    
    class IBat {
    public:
      IBat(Sc8812a* sc8812a) : sc8812a(sc8812a) { }
      operator float() {
        auto ibatValue = sc8812a->i2c[Reg::IBAT_VALUE];
        auto ibatValue2 = sc8812a->i2c[Reg::IBAT_VALUE_2] >> 6;
        return (4 * ibatValue + ibatValue2 + 1) * 2 / 1200. * 6 * 10 / sc8812a->kRs2;
      }
    private:
      Sc8812a* sc8812a;
    } iBat; //��λΪA
    
    
  private:
    void setCharingMode() {
      auto ctrl0Set = *i2c[Reg::CTRL0_SET];
        
      if (ctrl0Set & 1 << 7) { //���ڷŵ�ģʽ
        i2c[Reg::CTRL0_SET] = ctrl0Set & ~(1 << 7); //����Ϊ���ģʽ
      }
    }
    
    void setDischargingMode() {
      auto ctrl0Set = *i2c[Reg::CTRL0_SET];
        
      if (!(ctrl0Set & 1 << 7)) {
        i2c[Reg::CTRL0_SET] = ctrl0Set | (1 << 7);
      }
    }
    
    void setIBusLim(float iBus) { //��λΪA
      i2c[Reg::IBUS_LIM_SET] = iBus * 256 / kIBusRatio * kRs1 / 10 - 1;
    }
    
    void setIBatLim(float iBat) { //��λΪA
      i2c[Reg::IBAT_LIM_SET] = iBat * 256 / kIBatRatio * kRs2 / 10 - 1;
    }
    
    I2cDevice<0x74, Reg> i2c;
    static const int kRs1 = 10; //��λmOhm
    static const int kRs2 = 5;
    
    static const int kIBusRatio = 6;
    static const int kIBatRatio = 6;
  };
}