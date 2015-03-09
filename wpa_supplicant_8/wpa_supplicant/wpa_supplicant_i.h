/*
 * wpa_supplicant - Internal definitions
 * Copyright (c) 2003-2012, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef WPA_SUPPLICANT_I_H
#define WPA_SUPPLICANT_I_H

#include "utils/list.h"
#include "common/defs.h"
#include "common/sae.h"
#include "config_ssid.h"
extern const char *wpa_supplicant_version;
extern const char *wpa_supplicant_license;
#ifndef CONFIG_NO_STDOUT_DEBUG
extern const char *wpa_supplicant_full_license1;
extern const char *wpa_supplicant_full_license2;
extern const char *wpa_supplicant_full_license3;
extern const char *wpa_supplicant_full_license4;
extern const char *wpa_supplicant_full_license5;
#endif /* CONFIG_NO_STDOUT_DEBUG */

struct wpa_sm;
struct wpa_supplicant;
struct ibss_rsn;
struct scan_info;
struct wpa_bss;
struct wpa_scan_results;
struct hostapd_hw_modes;
struct wpa_driver_associate_params;

/*
 * Forward declarations of private structures used within the ctrl_iface
 * backends. Other parts of wpa_supplicant do not have access to data stored in
 * these structures.
 */
struct ctrl_iface_priv;
struct ctrl_iface_global_priv;
struct wpas_dbus_priv;

/**
 * struct wpa_interface - Parameters for wpa_supplicant_add_iface()
 */
struct wpa_interface {
	/**
	 * confname - Configuration name (file or profile) name
	 *
	 * This can also be %NULL when a configuration file is not used. In
	 * that case, ctrl_interface must be set to allow the interface to be
	 * configured.
	 */
	const char *confname;

	/**
	 * confanother - Additional configuration name (file or profile) name
	 *
	 * This can also be %NULL when the additional configuration file is not
	 * used.
	 */
	const char *confanother;

	/**
	 * ctrl_interface - Control interface parameter
	 *
	 * If a configuration file is not used, this variable can be used to
	 * set the ctrl_interface parameter that would have otherwise been read
	 * from the configuration file. If both confname and ctrl_interface are
	 * set, ctrl_interface is used to override the value from configuration
	 * file.
	 */
	const char *ctrl_interface;

	/**
	 * driver - Driver interface name, or %NULL to use the default driver
	 */
	const char *driver;

	/**
	 * driver_param - Driver interface parameters
	 *
	 * If a configuration file is not used, this variable can be used to
	 * set the driver_param parameters that would have otherwise been read
	 * from the configuration file. If both confname and driver_param are
	 * set, driver_param is used to override the value from configuration
	 * file.
	 */
	const char *driver_param;

	/**
	 * ifname - Interface name
	 */
	const char *ifname;

	/**
	 * bridge_ifname - Optional bridge interface name
	 *
	 * If the driver interface (ifname) is included in a Linux bridge
	 * device, the bridge interface may need to be used for receiving EAPOL
	 * frames. This can be enabled by setting this variable to enable
	 * receiving of EAPOL frames from an additional interface.
	 */
	const char *bridge_ifname;

	/**
	 * p2p_mgmt - Interface used for P2P management (P2P Device operations)
	 *
	 * Indicates whether wpas_p2p_init() must be called for this interface.
	 * This is used only when the driver supports a dedicated P2P Device
	 * interface that is not a network interface.
	 */
	int p2p_mgmt;
};

/**
 * struct wpa_params - Parameters for wpa_supplicant_init()
 */
struct wpa_params {
	/**
	 * daemonize - Run %wpa_supplicant in the background
	 */
	int daemonize;

	/**
	 * wait_for_monitor - Wait for a monitor program before starting
	 */
	int wait_for_monitor;

	/**
	 * pid_file - Path to a PID (process ID) file
	 *
	 * If this and daemonize are set, process ID of the background process
	 * will be written to the specified file.
	 */
	char *pid_file;

	/**
	 * wpa_debug_level - Debugging verbosity level (e.g., MSG_INFO)
	 */
	int wpa_debug_level;

	/**
	 * wpa_debug_show_keys - Whether keying material is included in debug
	 *
	 * This parameter can be used to allow keying material to be included
	 * in debug messages. This is a security risk and this option should
	 * not be enabled in normal configuration. If needed during
	 * development or while troubleshooting, this option can provide more
	 * details for figuring out what is happening.
	 */
	int wpa_debug_show_keys;

	/**
	 * wpa_debug_timestamp - Whether to include timestamp in debug messages
	 */
	int wpa_debug_timestamp;

	/**
	 * ctrl_interface - Global ctrl_iface path/parameter
	 */
	char *ctrl_interface;

	/**
	 * ctrl_interface_group - Global ctrl_iface group
	 */
	char *ctrl_interface_group;

	/**
	 * dbus_ctrl_interface - Enable the DBus control interface
	 */
	int dbus_ctrl_interface;

	/**
	 * wpa_debug_file_path - Path of debug file or %NULL to use stdout
	 */
	const char *wpa_debug_file_path;

	/**
	 * wpa_debug_syslog - Enable log output through syslog
	 */
	int wpa_debug_syslog;

	/**
	 * wpa_debug_tracing - Enable log output through Linux tracing
	 */
	int wpa_debug_tracing;

	/**
	 * override_driver - Optional driver parameter override
	 *
	 * This parameter can be used to override the driver parameter in
	 * dynamic interface addition to force a specific driver wrapper to be
	 * used instead.
	 */
	char *override_driver;

	/**
	 * override_ctrl_interface - Optional ctrl_interface override
	 *
	 * This parameter can be used to override the ctrl_interface parameter
	 * in dynamic interface addition to force a control interface to be
	 * created.
	 */
	char *override_ctrl_interface;

	/**
	 * entropy_file - Optional entropy file
	 *
	 * This parameter can be used to configure wpa_supplicant to maintain
	 * its internal entropy store over restarts.
	 */
	char *entropy_file;
};

struct p2p_srv_bonjour {
	struct dl_list list;
	struct wpabuf *query;
	struct wpabuf *resp;
};

struct p2p_srv_upnp {
	struct dl_list list;
	u8 version;
	char *service;
};

struct wpa_freq_range {
	unsigned int min;
	unsigned int max;
};

#if defined (CONFIG_MTK_P2P) || defined (CONFIG_WAPI_SUPPORT)

/* SIOCSIWENCODEEXT definitions */
#define IW_ENCODE_SEQ_MAX_SIZE	8
#define IW_ENCODE_EXT_GROUP_KEY		0x00000004
#define IW_ENCODE_EXT_SET_TX_KEY	0x00000008
#define IW_ENCODE_ALG_SMS4  0x20


/* WAPI */
struct	iw_encode_ext
{
	u32		ext_flags; /* IW_ENCODE_EXT_* */
	u8		tx_seq[IW_ENCODE_SEQ_MAX_SIZE]; /* LSB first */
	u8		rx_seq[IW_ENCODE_SEQ_MAX_SIZE]; /* LSB first */
	u8      addr[ETH_ALEN]; /* ff:ff:ff:ff:ff:ff for broadcast/multicast
	                         * (group) keys or unicast address for
			            * individual keys */
	u16		alg; /* IW_ENCODE_ALG_* */
	u16		key_len;
	u8		key[32];
};

struct wpa_driver_test_mode_info {
	u32  index;
	u32  buflen;
};
#ifdef CONFIG_MEDIATEK_WIFI_BEAM
struct wpa_driver_set_beamplus_params {
    struct wpa_driver_test_mode_info hdr;
	u32  value;
};
#endif
/* P2P Sigma*/
struct wpa_driver_p2p_sigma_params {
    struct wpa_driver_test_mode_info hdr;
    u32  idx;
	u32  value;
};

struct wpa_driver_testmode_params {
    struct wpa_driver_test_mode_info hdr;
    u8  *buf;
};    

struct wpa_driver_get_sta_statistics_params {
    struct wpa_driver_test_mode_info hdr;
    u32     version;
    u32     flag;
    u8      addr[ETH_ALEN];
    
    u8      *buf;   
};

struct wpa_driver_get_sta_link_detect_params {
    struct wpa_driver_test_mode_info hdr;    
    u8      *buf;   
};

/* Hotspot Client Management */
struct wpa_driver_hotspot_params {
    struct wpa_driver_test_mode_info hdr;
    u8    blocked;
    u8    bssid[ETH_ALEN];
};


/* SW CMD */
struct wpa_driver_sw_cmd_params {
    struct wpa_driver_test_mode_info hdr;
    u8               set;
    unsigned long    adr;
    unsigned long    data;
};


struct wpa_driver_wapi_key_params {
    struct wpa_driver_test_mode_info hdr;
    u8     key_index;
    u8     key_len;
    struct iw_encode_ext extparams;
};


#ifdef CONFIG_MTK_HS20R1

enum TestModeCmdType {
	TESTMODE_CMD_ID_SW_CMD = 1,
	TESTMODE_CMD_ID_WAPI = 2,
	TESTMODE_CMD_ID_HS20 = 3,
	NUM_OF_TESTMODE_CMD_ID
};

enum Hs20CmdType {
	HS20_CMD_ID_SET_BSSID_POOL = 0,
	NUM_OF_HS20_CMD_ID
};

struct param_hs20_set_bssid_pool {
	u8 fgBssidPoolIsEnable;
	u8 ucNumBssidPool;
	u8 arBssidPool[8][ETH_ALEN];
};


struct wpa_driver_hs20_data_s {
    struct wpa_driver_test_mode_info hdr;
	enum Hs20CmdType CmdType;
	
	struct param_hs20_set_bssid_pool hs20_set_bssid_pool;
};

#endif /* CONFIG_MTK_HS20R1 */


/* CONFIG_MTK_P2P */
struct wpa_driver_wfd_data_s {
    struct wpa_driver_test_mode_info hdr;
	u32  WfdCmdType;
	u8	 WfdEnable;
	u8	 WfdCoupleSinkStatus;
    u8   WfdSessionAvailable;
	u8   WfdSigmaMode;
	u16  WfdDevInfo;
	u16  WfdControlPort;
	u16  WfdMaximumTp;
	u16  WfdExtendCap;
	u8	 WfdCoupleSinkAddress[ETH_ALEN];
    u8   WfdAssociatedBssid[ETH_ALEN];
	u8	 WfdVideoIp[4];
	u8	 WfdAudioIp[4];
	u16  WfdVideoPort;
	u16  WfdAudioPort;
	u32  WfdFlag;
	u32  WfdPolicy; 
	u32  WfdState;
	u8	 WfdSessionInformationIE[24*8]; // Include Subelement ID, length
	u16  WfdSessionInformationIELen;
    u8   Reverved1[2];
    u8	 WfdPrimarySinkMac[ETH_ALEN];
    u8	 WfdSecondarySinkMac[ETH_ALEN];
    u32  WfdAdvancedFlag;
    /* Group 1 64 bytes */
    u8   WfdLocalIp[4];
    u16  WfdLifetimeAc2; /* Unit is 2 TU */
    u16  WfdLifetimeAc3; /* Unit is 2 TU */
    u16  WfdCounterThreshold;  /* Unit is ms */
    u8   Reverved2[54];
    /* Group 2 64 bytes */
	u8	 Reverved3[64];
    /* Group 3 64 bytes */
	u8	 Reverved4[64];
}wfd_data;

#endif

#ifdef CONFIG_MTK_P2P
#define WFD_FLAG_DEV_INFO_VALID (0x1<<0)
#define WFD_FLAG_COUPLED_SINK_INFO_VALID  (0x1<<1)
#define WFD_FLAG_ASSOC_BSSID_VALID (0x1<<2)
#define WFD_FLAG_EXT_CAPA_VALID (0x1<<3)


struct wfd_data_s {
    u32  WfdCmdType;
    u8   WfdEnable;
    u8   WfdCoupleSinkStatus;
    u8   WfdSessionAvailable;
    u8   WfdSigmaMode;
    u16  WfdDevInfo;
    u16  WfdControlPort;
    u16  WfdMaximumTp;
    u16  WfdExtendCap;
    u8   WfdCoupleSinkAddress[ETH_ALEN];
    u8   WfdAssociatedBssid[ETH_ALEN];
    u8   WfdVideoIp[4];
    u8   WfdAudioIp[4];
    u16  WfdVideoPort;
    u16  WfdAudioPort;
    u32  WfdFlag;
    u32  WfdPolicy; 
    u32  WfdState;
    u8   WfdSessionInformationIE[24*8]; // Include Subelement ID, length
    u16  WfdSessionInformationIELen;
    u8   Reverved1[2];
    u8   WfdPrimarySinkMac[ETH_ALEN];
    u8   WfdSecondarySinkMac[ETH_ALEN];
    u32  WfdAdvancedFlag ;
    /* Group 1 64 bytes */
    u8   WfdLocalIp[4];
    u16  WfdLifetimeAc2; /* Unit is 2 TU */
    u16  WfdLifetimeAc3; /* Unit is 2 TU */
    u8   Reverved2[56];
    /* Group 2 64 bytes */
	u8	 Reverved3[64];
    /* Group 3 64 bytes */
	u8	 Reverved4[64];
    u16  WfdDevInfoProbeRsp;
};
#endif
/**
 * struct wpa_global - Internal, global data for all %wpa_supplicant interfaces
 *
 * This structure is initialized by calling wpa_supplicant_init() when starting
 * %wpa_supplicant.
 */
struct wpa_global {
	struct wpa_supplicant *ifaces;
	struct wpa_params params;
	struct ctrl_iface_global_priv *ctrl_iface;
	struct wpas_dbus_priv *dbus;
	void **drv_priv;
	size_t drv_count;
	struct os_time suspend_time;
	struct p2p_data *p2p;
	struct wpa_supplicant *p2p_init_wpa_s;
	struct wpa_supplicant *p2p_group_formation;
	struct wpa_supplicant *p2p_invite_group;
	u8 p2p_dev_addr[ETH_ALEN];
	struct os_time p2p_go_wait_client;
	struct dl_list p2p_srv_bonjour; /* struct p2p_srv_bonjour */
	struct dl_list p2p_srv_upnp; /* struct p2p_srv_upnp */
	int p2p_disabled;
	int cross_connection;
	struct wpa_freq_range *p2p_disallow_freq;
	unsigned int num_p2p_disallow_freq;
	enum wpa_conc_pref {
		WPA_CONC_PREF_NOT_SET,
		WPA_CONC_PREF_STA,
		WPA_CONC_PREF_P2P
	} conc_pref;
	unsigned int p2p_cb_on_scan_complete:1;
	unsigned int p2p_per_sta_psk:1;

#ifdef CONFIG_WIFI_DISPLAY
	int wifi_display;
#define MAX_WFD_SUBELEMS 10
	struct wpabuf *wfd_subelem[MAX_WFD_SUBELEMS];
#endif /* CONFIG_WIFI_DISPLAY */

	struct psk_list_entry *add_psk; /* From group formation */
#ifdef CONFIG_MTK_P2P
    struct wfd_data_s wfd_data; 
    struct wfd_data_s wfd_data_cmd; /* information from command */
#endif /* CONFIG_MTK_WFD */
#ifdef CONFIG_MTK_HS20R1
	
#ifdef CONFIG_MTK_HS20R1_SIGMA
	struct hs20_sigma *sigma;
	int round;
	int query_round;
#endif /* CONFIG_MTK_HS20R1_SIGMA */

#endif /* CONFIG_MTK_HS20R1 */


};


/**
 * offchannel_send_action_result - Result of offchannel send Action frame
 */
enum offchannel_send_action_result {
	OFFCHANNEL_SEND_ACTION_SUCCESS /**< Frame was send and acknowledged */,
	OFFCHANNEL_SEND_ACTION_NO_ACK /**< Frame was sent, but not acknowledged
				       */,
	OFFCHANNEL_SEND_ACTION_FAILED /**< Frame was not sent due to a failure
				       */
};

struct wps_ap_info {
	u8 bssid[ETH_ALEN];
	enum wps_ap_info_type {
		WPS_AP_NOT_SEL_REG,
		WPS_AP_SEL_REG,
		WPS_AP_SEL_REG_OUR
	} type;
	unsigned int tries;
	struct os_time last_attempt;
};

struct wpa_ssid_value {
	u8 ssid[32];
	size_t ssid_len;
};

/**
 * struct wpa_supplicant - Internal data for wpa_supplicant interface
 *
 * This structure contains the internal data for core wpa_supplicant code. This
 * should be only used directly from the core code. However, a pointer to this
 * data is used from other files as an arbitrary context pointer in calls to
 * core functions.
 */
struct wpa_supplicant {
	struct wpa_global *global;
	struct wpa_supplicant *parent;
	struct wpa_supplicant *next;
	struct l2_packet_data *l2;
#ifdef CONFIG_WAPI_SUPPORT
	struct l2_packet_data *l2_wai;
	u8	bss_wapi_ie[64];
	u8	bss_wapi_ie_len;
#endif
	struct l2_packet_data *l2_br;
	unsigned char own_addr[ETH_ALEN];
	char ifname[100];
#ifdef CONFIG_CTRL_IFACE_DBUS
	char *dbus_path;
#endif /* CONFIG_CTRL_IFACE_DBUS */
#ifdef CONFIG_CTRL_IFACE_DBUS_NEW
	char *dbus_new_path;
	char *dbus_groupobj_path;
#ifdef CONFIG_AP
	char *preq_notify_peer;
#endif /* CONFIG_AP */
#endif /* CONFIG_CTRL_IFACE_DBUS_NEW */
	char bridge_ifname[16];

	char *confname;
	char *confanother;
	struct wpa_config *conf;
	int countermeasures;
	os_time_t last_michael_mic_error;
	u8 bssid[ETH_ALEN];
	u8 pending_bssid[ETH_ALEN]; /* If wpa_state == WPA_ASSOCIATING, this
				     * field contains the target BSSID. */
	int reassociate; /* reassociation requested */
	int disconnected; /* all connections disabled; i.e., do no reassociate
			   * before this has been cleared */
	struct wpa_ssid *current_ssid;
	struct wpa_bss *current_bss;
	int ap_ies_from_associnfo;
	unsigned int assoc_freq;

	/* Selected configuration (based on Beacon/ProbeResp WPA IE) */
	int pairwise_cipher;
	int group_cipher;
	int key_mgmt;
	int wpa_proto;
	int mgmt_group_cipher;
#ifdef CONFIG_WAPI_SUPPORT
	int proto;
#endif

	void *drv_priv; /* private data used by driver_ops */
	void *global_drv_priv;

	u8 *bssid_filter;
	size_t bssid_filter_count;

	u8 *disallow_aps_bssid;
	size_t disallow_aps_bssid_count;
	struct wpa_ssid_value *disallow_aps_ssid;
	size_t disallow_aps_ssid_count;

	enum { WPA_SETBAND_AUTO, WPA_SETBAND_5G, WPA_SETBAND_2G } setband;

	/* previous scan was wildcard when interleaving between
	 * wildcard scans and specific SSID scan when max_ssids=1 */
	int prev_scan_wildcard;
	struct wpa_ssid *prev_scan_ssid; /* previously scanned SSID;
					  * NULL = not yet initialized (start
					  * with wildcard SSID)
					  * WILDCARD_SSID_SCAN = wildcard
					  * SSID was used in the previous scan
					  */
#define WILDCARD_SSID_SCAN ((struct wpa_ssid *) 1)

	struct wpa_ssid *prev_sched_ssid; /* last SSID used in sched scan */
	int sched_scan_timeout;
	int sched_scan_interval;
	int first_sched_scan;
	int sched_scan_timed_out;

	void (*scan_res_handler)(struct wpa_supplicant *wpa_s,
				 struct wpa_scan_results *scan_res);
	struct dl_list bss; /* struct wpa_bss::list */
	struct dl_list bss_id; /* struct wpa_bss::list_id */
	size_t num_bss;
	unsigned int bss_update_idx;
	unsigned int bss_next_id;

	 /*
	  * Pointers to BSS entries in the order they were in the last scan
	  * results.
	  */
	struct wpa_bss **last_scan_res;
	unsigned int last_scan_res_used;
	unsigned int last_scan_res_size;
	int last_scan_full;
	struct os_time last_scan;

	struct wpa_driver_ops *driver;
	int interface_removed; /* whether the network interface has been
				* removed */
	struct wpa_sm *wpa;
	struct eapol_sm *eapol;

	struct ctrl_iface_priv *ctrl_iface;

	enum wpa_states wpa_state;
	int scanning;
	int sched_scanning;
	int new_connection;

	int eapol_received; /* number of EAPOL packets received after the
			     * previous association event */

	struct scard_data *scard;
	char imsi[20];
	int mnc_len;

	unsigned char last_eapol_src[ETH_ALEN];

	int keys_cleared;

	struct wpa_blacklist *blacklist;

	/**
	 * extra_blacklist_count - Sum of blacklist counts after last connection
	 *
	 * This variable is used to maintain a count of temporary blacklisting
	 * failures (maximum number for any BSS) over blacklist clear
	 * operations. This is needed for figuring out whether there has been
	 * failures prior to the last blacklist clear operation which happens
	 * whenever no other not-blacklisted BSS candidates are available. This
	 * gets cleared whenever a connection has been established successfully.
	 */
	int extra_blacklist_count;

	/**
	 * scan_req - Type of the scan request
	 */
	enum scan_req_type {
		/**
		 * NORMAL_SCAN_REQ - Normal scan request
		 *
		 * This is used for scans initiated by wpa_supplicant to find an
		 * AP for a connection.
		 */
		NORMAL_SCAN_REQ,

		/**
		 * INITIAL_SCAN_REQ - Initial scan request
		 *
		 * This is used for the first scan on an interface to force at
		 * least one scan to be run even if the configuration does not
		 * include any enabled networks.
		 */
		INITIAL_SCAN_REQ,

		/**
		 * MANUAL_SCAN_REQ - Manual scan request
		 *
		 * This is used for scans where the user request a scan or
		 * a specific wpa_supplicant operation (e.g., WPS) requires scan
		 * to be run.
		 */
		MANUAL_SCAN_REQ
	} scan_req;
	struct os_time scan_trigger_time;
	int scan_runs; /* number of scan runs since WPS was started */
	int *next_scan_freqs;
	int scan_interval; /* time in sec between scans to find suitable AP */
	int normal_scans; /* normal scans run before sched_scan */
	int scan_for_connection; /* whether the scan request was triggered for
				  * finding a connection */

	unsigned int drv_flags;
	unsigned int drv_enc;

	/*
	 * A bitmap of supported protocols for probe response offload. See
	 * struct wpa_driver_capa in driver.h
	 */
	unsigned int probe_resp_offloads;

	/* extended capabilities supported by the driver */
	const u8 *extended_capa, *extended_capa_mask;
	unsigned int extended_capa_len;

	int max_scan_ssids;
	int max_sched_scan_ssids;
	int sched_scan_supported;
	unsigned int max_match_sets;
	unsigned int max_remain_on_chan;
	unsigned int max_stations;

	int pending_mic_error_report;
	int pending_mic_error_pairwise;
	int mic_errors_seen; /* Michael MIC errors with the current PTK */

	struct wps_context *wps;
	int wps_success; /* WPS success event received */
	struct wps_er *wps_er;
	int blacklist_cleared;

	struct wpabuf *pending_eapol_rx;
	struct os_time pending_eapol_rx_time;
	u8 pending_eapol_rx_src[ETH_ALEN];
	unsigned int last_eapol_matches_bssid:1;

	struct ibss_rsn *ibss_rsn;

	int set_sta_uapsd;
	int sta_uapsd;
	int set_ap_uapsd;
	int ap_uapsd;

#ifdef CONFIG_SME
	struct {
		u8 ssid[32];
		size_t ssid_len;
		int freq;
		u8 assoc_req_ie[200];
		size_t assoc_req_ie_len;
		int mfp;
		int ft_used;
		u8 mobility_domain[2];
		u8 *ft_ies;
		size_t ft_ies_len;
		u8 prev_bssid[ETH_ALEN];
		int prev_bssid_set;
		int auth_alg;
		int proto;

		int sa_query_count; /* number of pending SA Query requests;
				     * 0 = no SA Query in progress */
		int sa_query_timed_out;
		u8 *sa_query_trans_id; /* buffer of WLAN_SA_QUERY_TR_ID_LEN *
					* sa_query_count octets of pending
					* SA Query transaction identifiers */
		struct os_time sa_query_start;
		u8 sched_obss_scan;
		u16 obss_scan_int;
		u16 bss_max_idle_period;
#ifdef CONFIG_SAE
		struct sae_data sae;
		struct wpabuf *sae_token;
		int sae_group_index;
#endif /* CONFIG_SAE */
	} sme;
#endif /* CONFIG_SME */

#ifdef CONFIG_AP
	struct hostapd_iface *ap_iface;
	void (*ap_configured_cb)(void *ctx, void *data);
	void *ap_configured_cb_ctx;
	void *ap_configured_cb_data;
#endif /* CONFIG_AP */

	unsigned int off_channel_freq;
	struct wpabuf *pending_action_tx;
	u8 pending_action_src[ETH_ALEN];
	u8 pending_action_dst[ETH_ALEN];
	u8 pending_action_bssid[ETH_ALEN];
	unsigned int pending_action_freq;
	int pending_action_no_cck;
	int pending_action_without_roc;
	void (*pending_action_tx_status_cb)(struct wpa_supplicant *wpa_s,
					    unsigned int freq, const u8 *dst,
					    const u8 *src, const u8 *bssid,
					    const u8 *data, size_t data_len,
					    enum offchannel_send_action_result
					    result);
	unsigned int roc_waiting_drv_freq;
	int action_tx_wait_time;

	int p2p_mgmt;

#ifdef CONFIG_P2P
	struct p2p_go_neg_results *go_params;
	int create_p2p_iface;
	u8 pending_interface_addr[ETH_ALEN];
	char pending_interface_name[100];
	int pending_interface_type;
	int p2p_group_idx;
	unsigned int pending_listen_freq;
	unsigned int pending_listen_duration;
	enum {
		NOT_P2P_GROUP_INTERFACE,
		P2P_GROUP_INTERFACE_PENDING,
		P2P_GROUP_INTERFACE_GO,
		P2P_GROUP_INTERFACE_CLIENT
	} p2p_group_interface;
	struct p2p_group *p2p_group;
	int p2p_long_listen; /* remaining time in long Listen state in ms */
#ifdef CONFIG_MTK_P2P_NFC	
	char p2p_pin[65];		// 32 * 2 + 1 for out of band password 
#else	
	char p2p_pin[10];
#endif /* CONFIG_MTK_P2P_NFC */	
	int p2p_wps_method;
	u8 p2p_auth_invite[ETH_ALEN];
	int p2p_sd_over_ctrl_iface;
	int p2p_in_provisioning;
	int pending_invite_ssid_id;
	int show_group_started;
	u8 go_dev_addr[ETH_ALEN];
	int pending_pd_before_join;
	u8 pending_join_iface_addr[ETH_ALEN];
	u8 pending_join_dev_addr[ETH_ALEN];
	int pending_join_wps_method;
	int p2p_join_scan_count;
	int auto_pd_scan_retry;
	int force_long_sd;
	u16 pending_pd_config_methods;
	enum {
		NORMAL_PD, AUTO_PD_GO_NEG, AUTO_PD_JOIN
	} pending_pd_use;

	/*
	 * Whether cross connection is disallowed by the AP to which this
	 * interface is associated (only valid if there is an association).
	 */
	int cross_connect_disallowed;

	/*
	 * Whether this P2P group is configured to use cross connection (only
	 * valid if this is P2P GO interface). The actual cross connect packet
	 * forwarding may not be configured depending on the uplink status.
	 */
	int cross_connect_enabled;

	/* Whether cross connection forwarding is in use at the moment. */
	int cross_connect_in_use;

	/*
	 * Uplink interface name for cross connection
	 */
	char cross_connect_uplink[100];

	unsigned int sta_scan_pending:1;
	unsigned int p2p_auto_join:1;
	unsigned int p2p_auto_pd:1;
	unsigned int p2p_persistent_group:1;
	unsigned int p2p_fallback_to_go_neg:1;
	unsigned int p2p_pd_before_go_neg:1;
	unsigned int p2p_go_ht40:1;
	unsigned int user_initiated_pd:1;
	int p2p_persistent_go_freq;
	int p2p_persistent_id;
	int p2p_go_intent;
	int p2p_connect_freq;
	struct os_time p2p_auto_started;
	struct wpa_ssid *p2p_last_4way_hs_fail;
#endif /* CONFIG_P2P */

	struct wpa_ssid *bgscan_ssid;
	const struct bgscan_ops *bgscan;
	void *bgscan_priv;

	const struct autoscan_ops *autoscan;
	struct wpa_driver_scan_params *autoscan_params;
	void *autoscan_priv;

	struct wpa_ssid *connect_without_scan;

	struct wps_ap_info *wps_ap;
	size_t num_wps_ap;
	int wps_ap_iter;

	int after_wps;
	int known_wps_freq;
	unsigned int wps_freq;
	u16 wps_ap_channel;
	int wps_fragment_size;
	int auto_reconnect_disabled;

	 /* Channel preferences for AP/P2P GO use */
	int best_24_freq;
	int best_5_freq;
	int best_overall_freq;

	struct gas_query *gas;

#ifdef CONFIG_INTERWORKING
	unsigned int fetch_anqp_in_progress:1;
	unsigned int network_select:1;
	unsigned int auto_select:1;
	unsigned int auto_network_select:1;
	unsigned int fetch_all_anqp:1;
	struct wpa_bss *interworking_gas_bss;
#endif /* CONFIG_INTERWORKING */
	unsigned int drv_capa_known;

	struct {
		struct hostapd_hw_modes *modes;
		u16 num_modes;
		u16 flags;
	} hw;

	int pno;

	/* WLAN_REASON_* reason codes. Negative if locally generated. */
	int disconnect_reason;

	struct ext_password_data *ext_pw;

	struct wpabuf *last_gas_resp;
	u8 last_gas_addr[ETH_ALEN];
	u8 last_gas_dialog_token;

	unsigned int no_keep_alive:1;
#ifdef CONFIG_MTK_HS20R1_SIGMA
	int reassociate_ppr1;
#endif

#ifdef CONFIG_MTK_WPS_NFC
	Boolean fgpubkey;
#endif /* CONFIG_MTK_WPS_NFC */

#ifdef CONFIG_MTK_P2P_NFC_FORCE_GO
	u32 force_p2p_go_intent;
#endif
#ifdef CONFIG_MTK_P2P_NFC
	u32 ip_alloc_need;
#endif
#ifdef CONFIG_WNM
	u8 wnm_dialog_token;
	u8 wnm_reply;
	u8 wnm_num_neighbor_report;
	u8 wnm_mode;
	u16 wnm_dissoc_timer;
	u8 wnm_validity_interval;
	u8 wnm_bss_termination_duration[12];
	struct neighbor_report *wnm_neighbor_report_elements;
#endif /* CONFIG_WNM */

#ifdef CONFIG_TESTING_GET_GTK
	u8 last_gtk[32];
	size_t last_gtk_len;
#endif /* CONFIG_TESTING_GET_GTK */

	unsigned int num_multichan_concurrent;
};


/* wpa_supplicant.c */
void wpa_supplicant_apply_ht_overrides(
	struct wpa_supplicant *wpa_s, struct wpa_ssid *ssid,
	struct wpa_driver_associate_params *params);
void wpa_supplicant_apply_vht_overrides(
	struct wpa_supplicant *wpa_s, struct wpa_ssid *ssid,
	struct wpa_driver_associate_params *params);

int wpa_set_wep_keys(struct wpa_supplicant *wpa_s, struct wpa_ssid *ssid);
int wpa_supplicant_set_wpa_none_key(struct wpa_supplicant *wpa_s,
				    struct wpa_ssid *ssid);

int wpa_supplicant_reload_configuration(struct wpa_supplicant *wpa_s);

const char * wpa_supplicant_state_txt(enum wpa_states state);
int wpa_supplicant_update_mac_addr(struct wpa_supplicant *wpa_s);
int wpa_supplicant_driver_init(struct wpa_supplicant *wpa_s);
int wpa_supplicant_set_suites(struct wpa_supplicant *wpa_s,
			      struct wpa_bss *bss, struct wpa_ssid *ssid,
			      u8 *wpa_ie, size_t *wpa_ie_len);
void wpa_supplicant_associate(struct wpa_supplicant *wpa_s,
			      struct wpa_bss *bss,
			      struct wpa_ssid *ssid);
void wpa_supplicant_set_non_wpa_policy(struct wpa_supplicant *wpa_s,
				       struct wpa_ssid *ssid);
void wpa_supplicant_initiate_eapol(struct wpa_supplicant *wpa_s);
void wpa_clear_keys(struct wpa_supplicant *wpa_s, const u8 *addr);
void wpa_supplicant_req_auth_timeout(struct wpa_supplicant *wpa_s,
				     int sec, int usec);
void wpa_supplicant_reinit_autoscan(struct wpa_supplicant *wpa_s);
void wpa_supplicant_set_state(struct wpa_supplicant *wpa_s,
			      enum wpa_states state);
struct wpa_ssid * wpa_supplicant_get_ssid(struct wpa_supplicant *wpa_s);
const char * wpa_supplicant_get_eap_mode(struct wpa_supplicant *wpa_s);
void wpa_supplicant_cancel_auth_timeout(struct wpa_supplicant *wpa_s);
void wpa_supplicant_deauthenticate(struct wpa_supplicant *wpa_s,
				   int reason_code);

void wpa_supplicant_enable_network(struct wpa_supplicant *wpa_s,
				   struct wpa_ssid *ssid);
void wpa_supplicant_disable_network(struct wpa_supplicant *wpa_s,
				    struct wpa_ssid *ssid);
void wpa_supplicant_select_network(struct wpa_supplicant *wpa_s,
				   struct wpa_ssid *ssid);
int wpa_supplicant_set_ap_scan(struct wpa_supplicant *wpa_s,
			       int ap_scan);
int wpa_supplicant_set_bss_expiration_age(struct wpa_supplicant *wpa_s,
					  unsigned int expire_age);
int wpa_supplicant_set_bss_expiration_count(struct wpa_supplicant *wpa_s,
					    unsigned int expire_count);
int wpa_supplicant_set_scan_interval(struct wpa_supplicant *wpa_s,
				     int scan_interval);
int wpa_supplicant_set_debug_params(struct wpa_global *global,
				    int debug_level, int debug_timestamp,
				    int debug_show_keys);
void free_hw_features(struct wpa_supplicant *wpa_s);

void wpa_show_license(void);

struct wpa_supplicant * wpa_supplicant_add_iface(struct wpa_global *global,
						 struct wpa_interface *iface);
int wpa_supplicant_remove_iface(struct wpa_global *global,
				struct wpa_supplicant *wpa_s,
				int terminate);
struct wpa_supplicant * wpa_supplicant_get_iface(struct wpa_global *global,
						 const char *ifname);
struct wpa_global * wpa_supplicant_init(struct wpa_params *params);
int wpa_supplicant_run(struct wpa_global *global);
void wpa_supplicant_deinit(struct wpa_global *global);

int wpa_supplicant_scard_init(struct wpa_supplicant *wpa_s,
			      struct wpa_ssid *ssid);
void wpa_supplicant_terminate_proc(struct wpa_global *global);
void wpa_supplicant_rx_eapol(void *ctx, const u8 *src_addr,
			     const u8 *buf, size_t len);
enum wpa_key_mgmt key_mgmt2driver(int key_mgmt);
void wpa_supplicant_update_config(struct wpa_supplicant *wpa_s);
void wpa_supplicant_clear_status(struct wpa_supplicant *wpa_s);
void wpas_connection_failed(struct wpa_supplicant *wpa_s, const u8 *bssid);
int wpas_driver_bss_selection(struct wpa_supplicant *wpa_s);
int wpas_is_p2p_prioritized(struct wpa_supplicant *wpa_s);
void wpas_auth_failed(struct wpa_supplicant *wpa_s);
void wpas_clear_temp_disabled(struct wpa_supplicant *wpa_s,
			      struct wpa_ssid *ssid, int clear_failures);
int disallowed_bssid(struct wpa_supplicant *wpa_s, const u8 *bssid);
int disallowed_ssid(struct wpa_supplicant *wpa_s, const u8 *ssid,
		    size_t ssid_len);
void wpas_request_connection(struct wpa_supplicant *wpa_s);
int wpas_build_ext_capab(struct wpa_supplicant *wpa_s, u8 *buf);
int wpas_wpa_is_in_progress(struct wpa_supplicant *wpa_s);

/**
 * wpa_supplicant_ctrl_iface_ctrl_rsp_handle - Handle a control response
 * @wpa_s: Pointer to wpa_supplicant data
 * @ssid: Pointer to the network block the reply is for
 * @field: field the response is a reply for
 * @value: value (ie, password, etc) for @field
 * Returns: 0 on success, non-zero on error
 *
 * Helper function to handle replies to control interface requests.
 */
int wpa_supplicant_ctrl_iface_ctrl_rsp_handle(struct wpa_supplicant *wpa_s,
					      struct wpa_ssid *ssid,
					      const char *field,
					      const char *value);

/* events.c */
void wpa_supplicant_mark_disassoc(struct wpa_supplicant *wpa_s);
int wpa_supplicant_connect(struct wpa_supplicant *wpa_s,
			   struct wpa_bss *selected,
			   struct wpa_ssid *ssid);
void wpa_supplicant_stop_countermeasures(void *eloop_ctx, void *sock_ctx);
void wpa_supplicant_delayed_mic_error_report(void *eloop_ctx, void *sock_ctx);
void wnm_bss_keep_alive_deinit(struct wpa_supplicant *wpa_s);
int wpa_supplicant_fast_associate(struct wpa_supplicant *wpa_s);
struct wpa_bss * wpa_supplicant_pick_network(struct wpa_supplicant *wpa_s,
					     struct wpa_ssid **selected_ssid);

/* eap_register.c */
int eap_register_methods(void);

/**
 * Utility method to tell if a given network is a persistent group
 * @ssid: Network object
 * Returns: 1 if network is a persistent group, 0 otherwise
 */
static inline int network_is_persistent_group(struct wpa_ssid *ssid)
{
	return ((ssid->disabled == 2) || ssid->p2p_persistent_group);
}

int wpas_network_disabled(struct wpa_supplicant *wpa_s, struct wpa_ssid *ssid);

int wpas_init_ext_pw(struct wpa_supplicant *wpa_s);

int get_shared_radio_freqs(struct wpa_supplicant *wpa_s,
			   int *freq_array, unsigned int len);
#ifdef CONFIG_MTK_HS20R1


#define WFA_IF_NAME_LEN   16
#define WFA_HS20_TYPE_LEN 16
#define WFA_HS20_LEN_256  256
#define WFA_HS20_LEN_32   32



#ifdef CONFIG_MTK_HS20R1_SIGMA

#define HS20_MAX_NUM_CREDENTIAL 8

struct hs20_credential {
	struct hs20_credential *prNext;
	
	char intf[WFA_IF_NAME_LEN];
	char type[WFA_HS20_TYPE_LEN];
	char username[WFA_HS20_LEN_32];
	char password[WFA_HS20_LEN_256];
	char imsi[WFA_HS20_LEN_32];
	char plmn_mnc[WFA_HS20_LEN_32];
	char plmn_mcc[WFA_HS20_LEN_32];
	char root_ca[WFA_HS20_LEN_32];
	char realm[WFA_HS20_LEN_32];
	u8	 prefer;
	char fqdn[WFA_HS20_LEN_32];
	char clientCA[WFA_HS20_LEN_32];
};

struct sigma_cmd_sta_cred_list {
	struct hs20_credential *prNext;
	size_t cred_count;
};

struct hs20_sigma {
	u8 **bssid_pool;
	size_t bssid_pool_count;
	// struct sigma_cmd_sta_bssid_pool *prBssidPool;
	//struct sigma_cmd_sta_add_credential arCred[HS20_MAX_NUM_CREDENTIAL];
	//size_t cred_count;
	struct sigma_cmd_sta_cred_list *prCredLst;
	u8 is_running;
	u8 is_connected;
	u8 assoc_backoff_time;
    u8 is_force_reassoc;
	// u8 *ns_ssid;
	// int ns_ssid_len;
	// u8 ns_bssid[ETH_ALEN];
};

#endif /* CONFIG_MTK_HS20R1_SIGMA */

#endif /* CONFIG_MTK_HS20R1 */

#endif /* WPA_SUPPLICANT_I_H */