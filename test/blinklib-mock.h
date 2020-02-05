
#ifndef HEADER_BLINKLIB_MOCK
#define HEADER_BLINKLIB_MOCK

    #include <string>

    typedef unsigned char uint8_t;
    typedef unsigned short uint16_t;
    typedef uint8_t byte;
    #define FACE_COUNT 6
    #define IR_DATAGRAM_LEN 16

    #define FOREACH_FACE(x) for(uint8_t x = 0; x < FACE_COUNT ; ++ x)
    #define COUNT_OF(x) ((sizeof(x)/sizeof(x[0])))

    #define Color std::string

    unsigned long millis();
    void setColorOnFace( Color newColor , byte face );
    void setColor( Color newColor);
    byte sin8_C( byte theta);
    Color dim( Color color, byte brightness);
    bool isValueReceivedOnFaceExpired(byte face);

    void sendDatagramOnFace( const byte *data, byte len , byte face );
    void markDatagramReadOnFace(byte face);
    bool isDatagramReadyOnFace(byte face);
    const byte getDatagramLengthOnFace(byte face);
    const byte* getDatagramOnFace(byte face);

    bool buttonDoubleClicked();
    bool buttonSingleClicked();
    bool didValueOnFaceChange(const byte face);
    bool isAlone();
    void setValueSentOnFace(const byte value, const byte face);
    void setValueSentOnAllFaces(const byte value);
    byte getLastValueReceivedOnFace(byte face);

    namespace blinklibMock{
        void triggerAlone();
        void resetMocks();
        void click();
        void expireValueOnFace(byte face);
        void expireValuesOnAllFaces();
        void expireValuesOnAllFacesExcept(byte face);
        void expireValuesOnAllFacesExceptBoth(byte face, byte other);
        void postDatagramOnFace(byte face, byte length);
        extern byte datagramPostBuffer[IR_DATAGRAM_LEN];
        void triggerChangeOnFace(const byte face);
        void setIncomingFaceValue(const byte face, const byte value);
        
    }

    #define RED "RED"
    #define ORANGE "ORANGE"
    #define YELLOW "YELLOW"
    #define GREEN "GREEN"
    #define CYAN "CYAN"
    #define BLUE "BLUE"
    #define MAGENTA "MAGENTA"
    #define WHITE "WHITE"
    #define OFF "OFF"

#endif