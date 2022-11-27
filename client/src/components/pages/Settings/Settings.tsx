import React, { HTMLProps, useCallback, useState } from "react";
import { useTranslation } from "react-i18next";
import { CSSTransition, TransitionGroup } from "react-transition-group";
import languageIcon from "../../../icones/menu/language.svg";
import settingsIcon from "../../../icones/menu/settings.svg";
import soundIcon from "../../../icones/menu/sound.svg";
import videoIcon from "../../../icones/menu/video.svg";
import style from "./style.scss";

const menus = [
  {
    translation: "PAGES.SETTINGS.MENU.CONTROL",
    icon: videoIcon
  },
  {
    translation: "PAGES.SETTINGS.MENU.LANGUAGES",
    icon: languageIcon
  },
  {
    translation: "PAGES.SETTINGS.MENU.SOUND",
    icon: soundIcon
  },
  {
    translation: "PAGES.SETTINGS.MENU.VIDEO",
    icon: videoIcon
  }
];

type Props = {
  id: string;
  navProperties?: HTMLProps<HTMLDivElement>;
};

const handleClick = (
  event: React.MouseEvent<HTMLButtonElement>,
  openUpdater: React.Dispatch<React.SetStateAction<boolean>>,
  currentState: boolean
) => {
  openUpdater(!currentState);
};

const changeLangBtnClick = (
  event: React.MouseEvent<HTMLButtonElement>,
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  i18n: any
): void => {
  if (i18n && typeof i18n === "object" && "changeLanguage" in i18n) {
    const foundIndex = i18n.languages.indexOf(i18n.language);
    const newIndex =
      foundIndex === i18n.languages.length - 1 ? 0 : foundIndex + 1;
    i18n.changeLanguage(i18n.languages[newIndex]);
  }
};

const transitionMenuClasses = {
  appear: style.fadeAppear,
  appearActive: style.fadeActiveAppear,
  appearDone: style.fadeDoneAppear,
  enter: style.fadeEnter,
  enterActive: style.fadeAtiveEnter,
  enterDone: style.fadeDoneEnter,
  exit: style.fadeExit,
  exitActive: style.fadeActiveExit,
  exitDone: style.fadeDoneExit
};

function Settings({ id, navProperties }: Props): React.ReactElement {
  const { i18n, t } = useTranslation();
  const [open, setOpen] = useState(false);
  const onClick = useCallback(
    (event: React.MouseEvent<HTMLButtonElement>) => {
      handleClick(event, setOpen, open);
    },
    [setOpen, open]
  );

  const onLanguageClick = useCallback(
    (event: React.MouseEvent<HTMLButtonElement>) => {
      changeLangBtnClick(event, i18n);
    },
    [i18n]
  );

  return (
    <nav id={id} className={style.SettingsContainer} {...navProperties}>
      <div className={style.details}>
        <button
          className={style.summary}
          data-alt={t("PAGES.SETTINGS.TITLE")}
          onClick={onClick}
        >
          <img src={settingsIcon} alt={`${t("PAGES.SETTINGS.TITLE")}`} />
        </button>

        <TransitionGroup className={style.menus}>
          {open
            ? menus.map((menu, index) => {
                return (
                  <CSSTransition
                    key={menu.translation}
                    timeout={200}
                    classNames={transitionMenuClasses}
                  >
                    <button
                      key={menu.translation}
                      className={style.item}
                      data-alt={`${t(menu.translation)}`}
                      onClick={index === 1 ? onLanguageClick : undefined}
                    >
                      <img src={menu.icon} alt={t(menu.translation)} />
                    </button>
                  </CSSTransition>
                );
              })
            : null}
        </TransitionGroup>
      </div>
    </nav>
  );
}

export default Settings;
