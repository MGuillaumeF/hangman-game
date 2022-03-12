import React from 'react'
import { useTranslation } from 'react-i18next'
import './style.scss'

const pageStyle : React.CSSProperties= { display : 'flex', flexDirection : 'column'}

type Props = {
    id:string
}

function Settings({id}: Props) {
    const {t} = useTranslation()
  return (
    <div id={id} style={pageStyle} className='settingsPage'>
        <h2>{t('pages.settings.title')}</h2>
        <button>{t('pages.settings.menu.control')}</button>
        <button>{t('pages.settings.menu.languages')}</button>
        <button>{t('pages.settings.menu.sound')}</button>
        <button>{t('pages.settings.menu.video')}</button>
    </div>
  )
}

export default Settings