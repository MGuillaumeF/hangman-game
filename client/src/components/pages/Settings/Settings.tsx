import React, { HTMLProps } from "react";
import { useTranslation } from "react-i18next";
import soundIcon from "../../../icones/menu/sound.svg";
import videoIcon from "../../../icones/menu/video.svg";
import style from "./style.scss";

type Props = {
  id: string;
  navProperties?: HTMLProps<HTMLDivElement>;
};

function Settings({ id, navProperties }: Props): React.ReactElement {
  const { t } = useTranslation();
  return (
    <nav id={id} className={style.SettingsContainer} {...navProperties}>
      <button>
        <img src={videoIcon} alt={t("PAGES.SETTINGS.MENU.CONTROL")} />
      </button>
      <button>
        <img src={videoIcon} alt={t("PAGES.SETTINGS.MENU.LANGUAGES")} />
      </button>
      <button>
        <img src={soundIcon} alt={t("PAGES.SETTINGS.MENU.SOUND")} />
      </button>
      <button>
        <img src={videoIcon} alt={t("PAGES.SETTINGS.MENU.VIDEO")} />
      </button>
    </nav>
  );
}

export default Settings;
