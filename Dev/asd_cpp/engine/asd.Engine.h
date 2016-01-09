﻿
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.CoreToEngine.h"
#include "ObjectSystem/asd.Scene.h"
#include "ObjectSystem/2D/asd.Layer2D.h"
#include "ObjectSystem/3D/asd.Layer3D.h"
#include "ObjectSystem/Transition/asd.Transition.h"
#include "asd.Engine.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

	/**
	@brief	初期化時に設定するオプションのクラス
	*/
	struct EngineOption
	{
		/**
		@brief	フルスクリーンで起動するか?
		*/
		bool			IsFullScreen = false;

		/**
		@brief	描画に使用するデバイス
		*/
		GraphicsDeviceType	GraphicsDevice = GraphicsDeviceType::Default;

		/**
		@brief	ウインドウの初期配置
		*/
		WindowPositionType	WindowPosition = WindowPositionType::Default;

		/**
		@brief	リソースの再読み込みを有効にするかどうか?
		*/
		bool IsReloadingEnabled = false;

		/**
		@brief	起動時に自動的に生成されるシーンとレイヤーの種類
		*/
		AutoGeneratedLayerType	AutoGeneratedLayer = AutoGeneratedLayerType::Layer2D;
	};

	class Engine
	{
	private:
		class SceneTransitionState
		{
		public:
			virtual void Proceed();
		};

		class NeutralState : public SceneTransitionState
		{
		};

		class FadingOutState : public SceneTransitionState
		{
		private:
			std::shared_ptr<Transition> m_transition;
			Scene::Ptr m_nextScene;

		public:
			void Proceed() override;
			FadingOutState(std::shared_ptr<Transition> transition, Scene::Ptr nextScene);
		};

		class FadingInState : public SceneTransitionState
		{
		private:
			std::shared_ptr<Transition> m_transition;
			Scene::Ptr m_previousScene;

		public:
			void Proceed() override;
			FadingInState(std::shared_ptr<Transition> transition, Scene::Ptr previousScene);
		};

		class QuicklyChangingState : public SceneTransitionState
		{
		private:
			Scene::Ptr m_nextScene;

		public:
			void Proceed() override;
			QuicklyChangingState(Scene::Ptr nextScene);
		};

	private:
		typedef std::shared_ptr<Scene> ScenePtr;

		static Core*					m_core;

		static Keyboard* m_keyboard ;
		static Mouse* m_mouse ;
		static JoystickContainer* m_joystickContainer ;
		static Log* m_logger ;
		static Profiler* m_profiler ;
		static LayerProfiler* m_layerProfiler;
		static Sound*	m_sound;
		static File*	m_file;
		static Graphics* m_graphics ;
		static ObjectSystemFactory* m_objectSystemFactory ;
		static AnimationSystem* m_animationSyatem ;


		static std::shared_ptr<Scene>	m_currentScene;
		static std::shared_ptr<Scene>	m_nextScene;
		static std::shared_ptr<Scene>	m_previousScene;

		static std::shared_ptr<Transition>	transition;
		static std::shared_ptr<SceneTransitionState> m_transitionState;

		static bool HasDLL(const char* path);
		static bool CheckDLL();
		static bool GenerateCore();

		Engine();
		~Engine();

	public:
		/**
			@brief	初期化を行う。
			@param	title	タイトル
			@param	width	横幅
			@param	height	縦幅
			@param	option	オプション
			@return	成否
		*/
		static bool Initialize(const achar* title, int32_t width, int32_t height, EngineOption option);

		/**
			@brief	初期化を行う。
			@param	handle1	ハンドル
			@param	handle2	ハンドル
			@param	width	横幅
			@param	height	縦幅
			@param	option	オプション
			@return	成否
		*/
		static bool InitializeByExternalWindow(void* handle1, void* handle2, int32_t width, int32_t height, EngineOption option);

		/**
		@brief	タイトルを設定する。
		@param	title	タイトル
		*/
		static void SetTitle(const achar* title);

		/**
			@brief	イベントを実行し、進行可否を判断する。
			@return	進行可能か?
		*/
		static bool DoEvents();

		/**
			@brief	更新処理を行う。
		*/
		static void Update();

		/**
			@brief	終了処理を行う。
		*/
		static void Terminate();

		/**
		@brief	マウスカーソルを作成する。
		@param	path	画像のパス
		@param	hot		カーソルの相対座標
		@return	カーソル
		*/
		static std::shared_ptr<Cursor> CreateCursor(const achar* path, Vector2DI hot);

		/**
		@brief	マウスカーソルを設定する。
		@param	cursor	カーソル
		*/
		static void SetCursor(std::shared_ptr<Cursor> cursor);

		/**
		@brief	クリップボードの内容を取得する。
		@return	クリップボード
		*/
		static const achar* GetClipboardString();

		/**
		@brief	クリップボードの内容を設定する。
		@param	s	文字列
		*/
		static void SetClipboardString(const achar* s);

		/**
		@brief	フルスクリーンモードかどうか設定する。
		@param	isFullscreenMode	フルスクリーンモードか
		@note
		現状、DirectXのみ有効である。
		*/
		static void SetIsFullscreenMode(bool isFullscreenMode);

		/**
			@brief	内部の参照数を取得する。
			@note
			内部オブジェクト間の参照の数を取得する。
			正しく内部オブジェクトが破棄されているか調査するためのデバッグ用である。
		*/
		static int32_t GetReferenceCount();

		/**
		@brief	一番最初に追加された2Dレイヤーにオブジェクトを追加する。
		@param	o	オブジェクト
		@return	成否
		*/
		static bool AddObject2D(std::shared_ptr<Object2D> o);

		/**
		@brief	一番最初に追加された2Dレイヤーからオブジェクトを削除する。
		@param	o	オブジェクト
		@return	成否
		*/
		static bool RemoveObject2D(std::shared_ptr<Object2D> o);

		/**
		@brief	一番最初に追加された3Dレイヤーにオブジェクトを追加する。
		@param	o	オブジェクト
		@return	成否
		*/
		static bool AddObject3D(std::shared_ptr<Object3D> o);

		/**
		@brief	一番最初に追加された3Dレイヤーからオブジェクトを削除する。
		@param	o	オブジェクト
		@return	成否
		*/
		static bool RemoveObject3D(std::shared_ptr<Object3D> o);

		/**
			@brief	描画する対象となるシーンを変更する。
			@param	scene	次のシーン
		*/
		static void ChangeScene(std::shared_ptr<Scene> scene);

		/**
		@brief	描画する対象となるシーンを画面遷移効果ありで変更する。
		@param	scene	次のシーン
		@param	transition	画面遷移効果
		*/
		static void ChangeSceneWithTransition(std::shared_ptr<Scene> scene, const std::shared_ptr<Transition>& transition);

		/**
		@brief	スクリーンショットを保存する。
		@param	path	出力先
		@note
		Windowsの場合、pngとjpg形式の保存に対応している。他のOSではpng形式の保存に対応している。
		形式の種類は出力先の拡張子で判断する。
		*/
		static void TakeScreenshot(const achar* path);

		/**
		@brief	スクリーンショットをgifアニメーションとして録画する。
		@param	path	出力先
		@param	frame	録画フレーム数
		@param	frequency_rate	録画頻度(例えば、1だと1フレームに1回保存、0.5だと2フレームに1回保存)
		@param	scale	ウインドウサイズに対する画像サイズの拡大率(ウインドウサイズが320の場合、0.5を指定すると160の画像が出力される)
		@note
		実行してから一定時間の間、録画を続ける。
		録画が終了するまでにアプリケーションが終了された場合、終了した時点までの録画結果が出力される。
		*/
		static void CaptureScreenAsGifAnimation(const achar* path, int32_t frame, float frequency_rate, float scale);

		/**
		@brief	1フレームで経過した実時間(秒)を取得する。
		@return	経過時間(秒)
		*/
		static float GetDeltaTime();

		/**
		@brief	1フレームで経過した時間を外部から設定する。
		@param	deltaTime	経過時間(秒)
		@note
		基本的に開発者は使用する必要はない。
		何らかの理由で無理やり経過時間を指定する場合に使用する。
		*/
		static void SetDeltaTime(float deltaTime);

		/**
		@brief	現在のFPSを取得する。
		@return FPS
		*/
		static float GetCurrentFPS();

		/**
		@brief	目標FPSを取得する。
		@return	FPS
		*/
		static int32_t GetTargetFPS();

		/**
		@brief	目標FPSを設定する。
		@param	fps	FPS
		*/
		static void SetTargetFPS(int32_t fps);

		/**
		@brief	時間を指定可能なオブジェクトの実時間あたりの進行速度を取得する。
		@return	進行速度
		*/
		static float GetTimeSpan();

		/**
		@brief	時間を指定可能なオブジェクトの実時間あたりの進行速度を設定する。
		@param	timeSpan	進行速度
		*/
		static void SetTimeSpan(float timeSpan);

		/**
		@brief	フレームレートの制御方法を取得する。
		@return	制御方法
		*/
		static FramerateMode GetFramerateMode();

		/**
		@brief	フレームレートの制御方法を設定する。
		@param	framerateMode	制御方法
		*/
		static void SetFramerateMode(FramerateMode framerateMode);

		/**
			@brief	ウインドウを閉じる。
		*/
		static void Close();

		/**
		@brief	Windowsの場合、ウインドウのハンドルを取得する。
		@return	ウインドウハンドル
		*/
		static void* GetWindowHandle();

		/**
		@brief キーボードクラスを取得する。
		@return キーボード
		*/
		static Keyboard* GetKeyboard();

		/**
		@brief マウスクラスを取得する。
		@return マウス
		*/
		static Mouse* GetMouse();

		/**
		@brief	ログクラスを取得する。
		@return	ログクラス
		*/
		static Log* GetLogger();

		/**
		@brief	プロファイラクラスを取得する。
		@return	プロファイラクラス
		*/
		static Profiler* GetProfiler();

		/**
		@brief	ジョイスティックコンテナクラスを取得する。
		@return	ジョイスティックコンテナクラス
		*/
		static JoystickContainer* GetJoystickContainer();
		
		/**
		@brief	ファイルクラスを取得する。
		@return	ファイルクラス
		*/
		static File* GetFile();

		/**
		@brief	Graphicsクラスを取得する。
		*/
		static Graphics* GetGraphics();

		/**
		@brief	Soundクラスを取得する。
		*/
		static Sound* GetSound();

		/**
		@brief	AnimationSyatemクラスを取得する。
		preturn	AnimationSyatemクラス
		*/
		static AnimationSystem* GetAnimationSyatem();

		/**
		@brief	ウィンドウのサイズを取得する。
		preturn	ウィンドウのサイズ
		*/
		static Vector2DI GetWindowSize();

		static bool GetProfilerVisibility();

		static void SetProfilerVisibility(bool visibility);

#if _WIN32

#else
		/**
			@brief	初期化を行う。
			@param	title	タイトル
			@param	width	横幅
			@param	height	縦幅
			@param	option	オプション
			@return	成否
		*/
		static bool Initialize(const wchar_t* title, int32_t width, int32_t height, EngineOption option)
		{
			return Initialize( ToAString(title).c_str(), width, height, option );
		}
#endif
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
